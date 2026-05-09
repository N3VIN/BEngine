#include "HealthComponent.h"
#include "SceneGraph/GameObject.h"

namespace dae {
    HealthComponent::HealthComponent(GameObject *gameObject, int lives)
        : Component(gameObject)
      , m_lives(lives) {}

    void HealthComponent::Die() {
        if (m_lives > 0) {
            --m_lives;
            // Notify(GetParent(), GameEvent::PlayerDied);
            OnLifeChanged.Broadcast(m_lives);
        }
    }

    int HealthComponent::GetLives() const {
        return m_lives;
    }
}
