#include "HealthComponent.h"

bomberman::HealthComponent::HealthComponent(bengine::GameObject *parent, int lives)
    : bengine::Component(parent)
  , m_lives(lives) {}

void bomberman::HealthComponent::Update(float deltaTime) {
    m_iframes.Update(deltaTime);
}

void bomberman::HealthComponent::TakeDamage(int amount, bengine::GameObject *attacker) {
    if (amount <= 0 || m_lives <= 0 || !m_iframes.IsExpired()) {
        return;
    }

    m_lastAttacker = attacker;
    m_lives -= amount;
    if (m_lives < 0) {
        m_lives = 0;
    }

    m_iframes.SetDuration(IFRAME_DURATION);
    m_onDamaged.Broadcast(m_lives);
}

bengine::GameObject *bomberman::HealthComponent::GetLastAttacker() const {
    return m_lastAttacker;
}

int bomberman::HealthComponent::GetLives() const {
    return m_lives;
}

bool bomberman::HealthComponent::IsAlive() const {
    return m_lives > 0;
}
