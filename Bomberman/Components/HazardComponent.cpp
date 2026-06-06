#include "HazardComponent.h"
#include "LevelGridComponent.h"
#include "GridMovementComponent.h"
#include "HealthComponent.h"
#include "Level/Tileset.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"
#include <algorithm>

bomberman::HazardComponent::HazardComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent)
    : bengine::Component(parent)
  , m_grid(gridComponent)
  , m_explosionLifetime(GetTileset().explosionLifetime) {
    const auto dims = m_grid->GetDimensions();
    m_hazardTime.resize(static_cast<size_t>(dims.x) * static_cast<size_t>(dims.y), 0.f);

    auto &bus = bengine::ServiceLocator::GetEventBus();

    m_explosionSub = bus.Subscribe<events::ExplosionAt>(
        [this](const events::ExplosionAt &event) {
            if (m_grid->InBounds(event.cell)) {
                m_hazardTime[Index(event.cell)] = m_explosionLifetime;
            }
        }
    );

    m_enemyKilledSub = bus.Subscribe<events::EnemyKilled>(
        [this](const events::EnemyKilled &event) {
            std::erase_if(m_enemies, [&event](const Damageable &enemy) {
                              return enemy.object == event.enemy;
                          }
            );
        }
    );
}

void bomberman::HazardComponent::Update(float deltaTime) {
    for (auto &remaining: m_hazardTime) {
        if (remaining > 0.f) {
            remaining = std::max(0.f, remaining - deltaTime);
        }
    }

    for (const auto &player: m_players) {
        const auto cell = player.movement->GetCell();
        const bool hit = ExplosionActive(cell) || std::ranges::any_of(
            m_enemies, [cell](const Damageable &enemy) {
                return enemy.health->IsAlive() && enemy.movement->GetCell() == cell; // check if player is on the enemy cell
            }
        );

        if (hit) {
            player.health->TakeDamage(1);
        }
    }

    for (const auto &enemy: m_enemies) {
        if (enemy.health->IsAlive() && ExplosionActive(enemy.movement->GetCell())) {
            enemy.health->TakeDamage(1);
        }
    }
}

void bomberman::HazardComponent::RegisterPlayer(bengine::GameObject *player) {
    m_players.push_back({
            .object = player,
            .movement = player->GetComponent<GridMovementComponent>(),
            .health = player->GetComponent<HealthComponent>(),
        }
    );
}

void bomberman::HazardComponent::RegisterEnemy(bengine::GameObject *enemy) {
    m_enemies.push_back({
            .object = enemy,
            .movement = enemy->GetComponent<GridMovementComponent>(),
            .health = enemy->GetComponent<HealthComponent>(),
        }
    );
}

bool bomberman::HazardComponent::ExplosionActive(glm::ivec2 cell) const {
    return m_grid->InBounds(cell) && m_hazardTime[Index(cell)] > 0.f;
}

size_t bomberman::HazardComponent::Index(glm::ivec2 cell) const {
    return static_cast<size_t>(cell.y) * static_cast<size_t>(m_grid->GetDimensions().x) + static_cast<size_t>(cell.x);
}
