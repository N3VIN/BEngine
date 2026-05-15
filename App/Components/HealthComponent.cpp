#include "HealthComponent.h"
#include "SceneGraph/GameObject.h"
#include "Patterns/ServiceLocator.h"
#include "GameEvents.h"

namespace dae {
    HealthComponent::HealthComponent(GameObject *gameObject, int lives)
        : Component(gameObject)
      , m_lives(lives) {}

    void HealthComponent::Die() {
        if (m_lives <= 0) {
            return;
        }

        --m_lives;

        ServiceLocator::GetEventBus().Broadcast(events::PlayerDamaged{
                .player = GetParent(),
                .newLives = m_lives,
            }
        );
    }

    int HealthComponent::GetLives() const {
        return m_lives;
    }

    GameObject *HealthComponent::GetOwner() const {
        return GetParent();
    }
}
