#include "TimerComponent.h"
#include "Gameplay/GameEvents.h"
#include "SceneGraph/GameObject.h"
#include "Components/SpriteTextComponent.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

bomberman::TimerComponent::TimerComponent(bengine::GameObject *owner, float seconds)
    : bengine::Component(owner)
  , m_text(owner->GetComponent<bengine::SpriteTextComponent>())
  , m_timer(seconds) {
    Refresh(static_cast<int>(std::ceil(seconds)));
}

void bomberman::TimerComponent::Update(float deltaTime) {
    if (m_expired) {
        return;
    }

    m_timer.Update(deltaTime);

    const int seconds = static_cast<int>(std::ceil(m_timer.GetRemaining()));
    if (seconds != m_displayedSeconds) {
        m_displayedSeconds = seconds;
        Refresh(m_displayedSeconds);
    }

    if (m_timer.IsExpired()) {
        m_expired = true;
        bengine::ServiceLocator::GetEventBus().Broadcast(events::TimeExpired{});
    }
}

void bomberman::TimerComponent::Refresh(int seconds) const {
    m_text->SetText("TIME " + std::to_string(seconds));
}
