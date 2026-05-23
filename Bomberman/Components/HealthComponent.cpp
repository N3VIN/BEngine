#include "HealthComponent.h"
#include "GridMovementComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"

bomberman::HealthComponent::HealthComponent(bengine::GameObject *parent, int lives)
    : bengine::Component(parent)
  , m_lives(lives) {
    auto *movement = parent->GetComponent<GridMovementComponent>();
    m_explosionSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::ExplosionAt>(
        [this, movement](const events::ExplosionAt &event) {
            if (movement && movement->GetCell() == event.cell) {
                TakeDamage(1);
            }
        }
    );
}

void bomberman::HealthComponent::TakeDamage(int amount) {
    if (amount <= 0 || m_lives <= 0) {
        return;
    }

    m_lives -= amount;
    if (m_lives < 0) {
        m_lives = 0;
    }

    bengine::ServiceLocator::GetEventBus().Broadcast(events::PlayerDamaged{
        .player = GetParent(),
        .newLives = m_lives,
    });
}

int bomberman::HealthComponent::GetLives() const {
    return m_lives;
}
