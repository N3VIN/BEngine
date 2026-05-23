#include "HealthComponent.h"
#include "SceneGraph/GameObject.h"
#include "Patterns/ServiceLocator.h"
#include "GameEvents.h"
app::HealthComponent::HealthComponent(bengine::GameObject *gameObject, int lives)
    : bengine::Component(gameObject)
  , m_lives(lives) {}

void app::HealthComponent::Die() {
    if (m_lives <= 0) {
        return;
    }

    --m_lives;
    bengine::ServiceLocator::GetEventBus().Broadcast(events::PlayerDamaged{
            .player = GetParent(),
            .newLives = m_lives,
        }
    );
}

int app::HealthComponent::GetLives() const {
    return m_lives;
}

bengine::GameObject *app::HealthComponent::GetOwner() const {
    return GetParent();
}
