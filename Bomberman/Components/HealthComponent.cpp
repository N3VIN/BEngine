#include "HealthComponent.h"
#include "GridMovementComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"

namespace dae {
    HealthComponent::HealthComponent(GameObject *parent, int lives)
        : Component(parent)
      , m_lives(lives) {
        auto *movement = parent->GetComponent<GridMovementComponent>();
        m_explosionSub = ServiceLocator::GetEventBus().Subscribe<events::ExplosionAt>(
            [this, movement](const events::ExplosionAt &event) {
                if (movement && movement->GetCell() == event.cell) {
                    TakeDamage(1);
                }
            }
        );
    }

    void HealthComponent::TakeDamage(int amount) {
        if (amount <= 0 || m_lives <= 0) {
            return;
        }

        m_lives -= amount;
        if (m_lives < 0) {
            m_lives = 0;
        }

        ServiceLocator::GetEventBus().Broadcast(events::PlayerDamaged{
            .player = GetParent(),
            .newLives = m_lives,
        });
    }

    int HealthComponent::GetLives() const {
        return m_lives;
    }
}
