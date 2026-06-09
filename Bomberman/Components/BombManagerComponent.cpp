#include "BombManagerComponent.h"
#include "BombComponent.h"
#include "ExplosionComponent.h"
#include "BrickComponent.h"
#include "LevelGridComponent.h"
#include "SpriteRendererComponent.h"
#include "HealthComponent.h"
#include "Level/Tileset.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"

bomberman::BombManagerComponent::BombManagerComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent)
    : bengine::Component(parent)
  , m_gridComponent(gridComponent) {
    const auto dims = m_gridComponent->GetDimensions();
    m_bombAtCell.resize(static_cast<size_t>(dims.x) * static_cast<size_t>(dims.y), nullptr);

    m_pickupSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::PickupCollected>(
        [this](const events::PickupCollected &event) {
            switch (event.type) {
                case PickupType::BombUp:
                    AddBomb(event.player);
                    break;
                case PickupType::FlameUp:
                    AddFlame(event.player);
                    break;
                case PickupType::Detonator:
                    GiveDetonator(event.player);
                    break;
            }
        }
    );
}

void bomberman::BombManagerComponent::RegisterPlayer(bengine::GameObject *player) {
    m_playerStats[player] = {};

    auto *health = player->GetComponent<HealthComponent>();
    m_damagedSubs.push_back(health->SubscribeDamaged([this, player](int) {
                m_playerStats[player].hasDetonator = false; // lose detonator on death
            }
        )
    );
}

void bomberman::BombManagerComponent::Update(float deltaTime) {
    if (m_activeBombs.empty()) {
        return;
    }

    std::vector<BombComponent *> expired;
    for (auto *bomb: m_activeBombs) {
        bomb->TickFuse(deltaTime);
        if (bomb->FuseExpired()) {
            expired.push_back(bomb);
        }
    }

    for (auto *bomb: expired) {
        DetonateBomb(bomb);
    }
}

void bomberman::BombManagerComponent::PlaceBomb(glm::ivec2 cell, bengine::GameObject *owner) {
    if (!m_gridComponent->InBounds(cell)) {
        return;
    }

    if (BombAt(cell)) {
        return;
    }

    if (!m_gridComponent->IsWalkable(cell)) {
        return;
    }

    const auto it = m_playerStats.find(owner);
    if (it == m_playerStats.end()) {
        return;
    }

    PlayerBombStats &stats = it->second;
    if (stats.activeBombs >= stats.maxBombs) {
        return;
    }

    ++stats.activeBombs;

    auto bombGO = std::make_unique<bengine::GameObject>();
    bombGO->SetLocalPosition(m_gridComponent->CellToWorld(cell));

    bombGO->AddComponent<SpriteRendererComponent>(SpriteType::Bomb);

    auto *bomb = bombGO->AddComponent<BombComponent>(cell, stats.blastRadius, owner, BOMB_FUSE_DURATION, stats.hasDetonator);

    m_gridComponent->SetWall(cell, true);
    const auto idx = BombIndex(cell);
    m_activeBombs.push_back(bomb);
    m_bombAtCell[idx] = bengine::GetActiveScene()->Add(std::move(bombGO));
}

void bomberman::BombManagerComponent::DetonateBomb(BombComponent *bomb) {
    if (bomb->IsDetonated()) {
        return;
    }

    bomb->MarkDetonated();
    std::erase(m_activeBombs, bomb);

    const glm::ivec2 cell = bomb->GetCell();
    const int radius = bomb->GetRadius();
    auto *owner = bomb->GetOwner();
    const auto idx = BombIndex(cell);

    m_gridComponent->SetWall(cell, false);
    m_bombAtCell[idx] = nullptr;
    bengine::GetActiveScene()->Remove(bomb->GetGameObject());

    const auto it = m_playerStats.find(owner);
    if (it != m_playerStats.end() && it->second.activeBombs > 0) {
        --it->second.activeBombs;
    }

    SpawnExplosionAt(cell);
    bengine::ServiceLocator::GetEventBus().Broadcast(events::ExplosionAt{cell});

    SpreadInDirection(cell, {0, -1}, radius);
    SpreadInDirection(cell, {0, 1}, radius);
    SpreadInDirection(cell, {-1, 0}, radius);
    SpreadInDirection(cell, {1, 0}, radius);

    bengine::ServiceLocator::GetEventBus().Broadcast(events::BombDetonated{cell});

    ProcessDetonationQueue();
}

void bomberman::BombManagerComponent::AddBomb(bengine::GameObject *owner) {
    if (const auto it = m_playerStats.find(owner); it != m_playerStats.end() && it->second.maxBombs < MAX_BOMBS) {
        ++it->second.maxBombs;
    }
}

void bomberman::BombManagerComponent::AddFlame(bengine::GameObject *owner) {
    if (const auto it = m_playerStats.find(owner); it != m_playerStats.end() && it->second.blastRadius < MAX_BLAST_RADIUS) {
        ++it->second.blastRadius;
    }
}

void bomberman::BombManagerComponent::GiveDetonator(bengine::GameObject *owner) {
    if (const auto it = m_playerStats.find(owner); it != m_playerStats.end()) {
        it->second.hasDetonator = true;
    }
}

void bomberman::BombManagerComponent::DetonateOldestBomb(bengine::GameObject *owner) {
    const auto it = m_playerStats.find(owner);
    if (it == m_playerStats.end() || !it->second.hasDetonator) {
        return;
    }

    for (auto *bomb: m_activeBombs) {
        if (bomb->GetOwner() == owner) {
            DetonateBomb(bomb);
            return;
        }
    }
}

void bomberman::BombManagerComponent::SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range) {
    auto &bus = bengine::ServiceLocator::GetEventBus();

    for (int distance = 1; distance <= range; ++distance) {
        const glm::ivec2 cell = origin + direction * distance;

        if (!m_gridComponent->InBounds(cell)) {
            break;
        }

        if (m_gridComponent->HasWall(cell)) {
            break;
        }

        if (auto *chainBombGO = BombAt(cell)) {
            if (auto *chainBomb = chainBombGO->GetComponent<BombComponent>()) {
                m_detonationQueue.push_back(chainBomb);
            }

            break;
        }

        if (m_gridComponent->IsBrick(cell)) {
            if (auto *tile = m_gridComponent->DestroyBrick(cell)) {
                if (auto *brick = tile->GetComponent<BrickComponent>()) {
                    brick->Destroy();
                }

                bus.Broadcast(events::BrickDestroyed{cell});
            }

            break;
        }

        bus.Broadcast(events::ExplosionAt{cell});
    }
}

void bomberman::BombManagerComponent::SpawnExplosionAt(glm::ivec2 cell) const {
    if (!m_gridComponent->InBounds(cell)) {
        return;
    }

    const auto &tileset = GetTileset();
    const float lifetime = tileset.explosionLifetime;

    auto explosionGO = std::make_unique<bengine::GameObject>();
    const float cellSize = m_gridComponent->GetCellSize();
    const float offsetX = -static_cast<float>(tileset.explosion.numCols / 2) * cellSize;
    const float offsetY = -static_cast<float>(tileset.explosion.numRows / 2) * cellSize;
    explosionGO->SetLocalPosition(m_gridComponent->CellToWorld(cell) + glm::vec2{offsetX, offsetY});

    explosionGO->AddComponent<SpriteRendererComponent>(SpriteType::Explosion);
    explosionGO->AddComponent<ExplosionComponent>(lifetime);

    bengine::GetActiveScene()->Add(std::move(explosionGO));
}

void bomberman::BombManagerComponent::ProcessDetonationQueue() {
    while (!m_detonationQueue.empty()) {
        auto *bomb = m_detonationQueue.back();
        m_detonationQueue.pop_back();
        DetonateBomb(bomb);
    }
}

size_t bomberman::BombManagerComponent::BombIndex(glm::ivec2 cell) const {
    return static_cast<size_t>(cell.y) * static_cast<size_t>(m_gridComponent->GetDimensions().x) + static_cast<size_t>(cell.x);
}

bengine::GameObject *bomberman::BombManagerComponent::BombAt(glm::ivec2 cell) const {
    if (!m_gridComponent->InBounds(cell)) {
        return nullptr;
    }

    return m_bombAtCell[BombIndex(cell)];
}
