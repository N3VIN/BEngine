#include "LivesDisplayComponent.h"
#include "SceneGraph/GameObject.h"
#include "HealthComponent.h"
#include "Components/TextComponent.h"
#include "Patterns/ServiceLocator.h"
#include "GameEvents.h"
app::LivesDisplayComponent::LivesDisplayComponent(bengine::GameObject *owner, HealthComponent *healthComponent)
    : bengine::Component(owner)
  , m_textComponent(owner->GetComponent<bengine::TextComponent>())
  , m_healthComponent(healthComponent) {
    if (!m_healthComponent) {
        return;
    }

    m_textComponent->SetText("Lives: " + std::to_string(m_healthComponent->GetLives()));

    m_damageSD = bengine::ServiceLocator::GetEventBus().Subscribe<events::PlayerDamaged>(
        [this](const events::PlayerDamaged &event) {
            if (event.player == m_healthComponent->GetOwner()) {
                m_textComponent->SetText("Lives: " + std::to_string(event.newLives));
            }
        }
    );
}
