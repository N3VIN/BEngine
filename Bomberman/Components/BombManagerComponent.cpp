#include "BombManagerComponent.h"
#include "BombComponent.h"
#include "LevelGridComponent.h"
#include "GridMovementComponent.h"
#include "HealthComponent.h"
#include "../Level/Tileset.h"
#include "../../Core/SceneGraph/Scene.h"
#include "../../Core/SceneGraph/GameObject.h"
#include "../../Core/Components/RenderComponent.h"
#include "../../Core/Renderer/Renderer.h"
#include "../../Core/Renderer/Texture2D.h"
#include "../../Core/Renderer/ResourceManager.h"
#include "../../Core/Components/CameraComponent.h"

namespace dae {
    BombManagerComponent::BombManagerComponent(GameObject *parent, Scene &scene, LevelGridComponent *gridComponent)
        : Component(parent)
      , m_scene(scene)
      , m_grid(gridComponent) {
        const auto dims = m_grid->GetDimensions();
        m_bombAtCell.resize(static_cast<size_t>(dims.x) * static_cast<size_t>(dims.y), nullptr);
    }

    void BombManagerComponent::RegisterPlayer(GameObject *player) {
        m_playerBombCount[player] = 0;
        m_damageables.push_back(player);
    }

    void BombManagerComponent::RegisterDamageable(GameObject *object) {
        m_damageables.push_back(object);
    }

    void BombManagerComponent::PlaceBomb(glm::ivec2 cell, GameObject *owner) {
        if (!m_grid->InBounds(cell)) {
            return;
        }

        if (BombAt(cell)) {
            return;
        }

        if (!m_grid->IsWalkable(cell)) {
            return;
        }

        if (const auto it = m_playerBombCount.find(owner); it != m_playerBombCount.end() && it->second >= m_maxBombsPerPlayer) {
            return;
        }

        ++m_playerBombCount[owner];

        auto bombGO = std::make_unique<GameObject>();
        bombGO->SetLocalPosition(m_grid->CellToWorld(cell));

        const auto &tileset = GetTileset();
        auto *renderComp = bombGO->AddComponent<RenderComponent>();
        renderComp->SetTexture(tileset.bombTexturePath);
        renderComp->SetSourceRect(SDL_Rect{
                tileset.bombCoord.x * tileset.tileSize,
                tileset.bombCoord.y * tileset.tileSize,
                tileset.tileSize,
                tileset.tileSize
            }
        );

        renderComp->SetScale(tileset.spriteScale);

        bombGO->AddComponent<BombComponent>(this, m_grid, cell, owner, m_fuseTime);

        m_grid->SetWall(cell, true);
        m_bombAtCell[BombIndex(cell)] = bombGO->GetComponent<BombComponent>();
        m_scene.Add(std::move(bombGO));
    }

    void BombManagerComponent::DetonateBomb(BombComponent *bomb) {
        const glm::ivec2 cell = bomb->GetCell();
        auto *owner = bomb->GetOwner();

        m_grid->SetWall(cell, false);
        m_bombAtCell[BombIndex(cell)] = nullptr;
        m_scene.Remove(bomb->GetGameObject());

        const auto it = m_playerBombCount.find(owner);
        if (it != m_playerBombCount.end() && it->second > 0) {
            --it->second;
        }

        SpawnExplosionAt(cell);

        SpreadInDirection(cell, {0, -1}, BLAST_RADIUS);
        SpreadInDirection(cell, {0, 1}, BLAST_RADIUS);
        SpreadInDirection(cell, {-1, 0}, BLAST_RADIUS);
        SpreadInDirection(cell, {1, 0}, BLAST_RADIUS);

        ProcessDetonationQueue(); // if any is present in the queue
    }

    void BombManagerComponent::SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range) {
        for (int distance = 1; distance <= range; ++distance) {
            const glm::ivec2 cell = origin + direction * distance;

            if (!m_grid->InBounds(cell)) {
                break;
            }

            if (m_grid->HasWall(cell)) {
                break;
            }

            if (auto *chainBomb = BombAt(cell)) {
                m_detonationQueue.push_back(chainBomb);
                break;
            }

            if (m_grid->IsBrick(cell)) {
                if (auto *tile = m_grid->DestroyBrick(cell)) {
                    m_scene.Remove(tile);
                }
                break;
            }

            for (const auto *objects: m_damageables) {
                if (const auto *movement = objects->GetComponent<GridMovementComponent>(); movement && movement->GetCell() == cell) {
                    if (auto *health = objects->GetComponent<HealthComponent>()) {
                        health->TakeDamage(1);
                    }
                }
            }
        }
    }

    void BombManagerComponent::SpawnExplosionAt(glm::ivec2 cell) {
        const float lifetime = GetTileset().explosionLifetime;
        for (auto &explosion: m_activeExplosions) {
            if (explosion.cell == cell) {
                explosion.timer.SetDuration(lifetime);
                return;
            }
        }

        m_activeExplosions.push_back({cell, Timer{lifetime}});
        OnExplosionAt.Broadcast(cell);
    }

    void BombManagerComponent::ProcessDetonationQueue() {
        while (!m_detonationQueue.empty()) {
            auto *bomb = m_detonationQueue.back();
            m_detonationQueue.pop_back();
            bomb->Detonate();
        }
    }

    size_t BombManagerComponent::BombIndex(glm::ivec2 cell) const {
        return static_cast<size_t>(cell.y) * static_cast<size_t>(m_grid->GetDimensions().x) + static_cast<size_t>(cell.x);
    }

    BombComponent *BombManagerComponent::BombAt(glm::ivec2 cell) const {
        if (!m_grid->InBounds(cell)) {
            return nullptr;
        }

        return m_bombAtCell[BombIndex(cell)];
    }

    void BombManagerComponent::Update(float deltaTime) {
        for (auto &explosion: m_activeExplosions) {
            explosion.timer.Update(deltaTime);
        }
        std::erase_if(m_activeExplosions, [](const ActiveExplosion &explosion) {
                          return explosion.timer.IsExpired();
                      }
        );
    }

    void BombManagerComponent::Render() const {
        if (m_activeExplosions.empty()) {
            return;
        }

        const auto &tileset = GetTileset();
        const auto explosionTexture = ResourceManager::GetInstance().LoadTexture(tileset.bombTexturePath);
        if (!explosionTexture) {
            return;
        }

        const int explosionTopLeftCol = tileset.explosionCenter.x - tileset.explosionCells / 2;
        const int explosionTopLeftRow = tileset.explosionCenter.y - tileset.explosionCells / 2;
        const SDL_Rect explosionSrcRect{
            explosionTopLeftCol * tileset.tileSize,
            explosionTopLeftRow * tileset.tileSize,
            tileset.explosionCells * tileset.tileSize,
            tileset.explosionCells * tileset.tileSize
        };

        const glm::vec2 explosionOffset(-static_cast<float>(tileset.explosionCells / 2) * m_grid->GetCellSize());

        const auto &renderer = Renderer::GetInstance();
        const auto *camera = renderer.GetActiveCamera();

        for (const auto &explosion: m_activeExplosions) {
            glm::vec2 worldPos = m_grid->CellToWorld(explosion.cell) + explosionOffset;
            float scale = tileset.spriteScale;

            if (camera) {
                worldPos = renderer.WorldToScreen(worldPos);
                scale *= camera->GetZoom();
            }

            const SDL_FRect dst{
                worldPos.x,
                worldPos.y,
                static_cast<float>(explosionSrcRect.w) * scale,
                static_cast<float>(explosionSrcRect.h) * scale
            };

            renderer.RenderTexture(*explosionTexture, dst, &explosionSrcRect, 0.0f);
        }
    }
}
