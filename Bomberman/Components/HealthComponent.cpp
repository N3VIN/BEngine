#include "HealthComponent.h"
#include "../../Core/SceneGraph/GameObject.h"

namespace dae {
    HealthComponent::HealthComponent(GameObject *parent, int lives)
        : Component(parent)
      , m_lives(lives) {}

    void HealthComponent::TakeDamage(int amount) {
        if (amount <= 0 || m_lives <= 0) {
            return;
        }

        m_lives -= amount;
        if (m_lives < 0) {
            m_lives = 0;
        }

        OnLifeChanged.Broadcast(m_lives);
    }

    int HealthComponent::GetLives() const {
        return m_lives;
    }
}
