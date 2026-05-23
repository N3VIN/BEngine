#include "ScoreDisplayComponent.h"
#include "SceneGraph/GameObject.h"
#include "PickupComponent.h"
#include "Components/TextComponent.h"
app::ScoreDisplayComponent::~ScoreDisplayComponent() {
    if (m_pickupComponent) {
        m_pickupComponent->RemoveObserver(this);
    }
}

app::ScoreDisplayComponent::ScoreDisplayComponent(bengine::GameObject *owner)
    : bengine::Component(owner)
  , m_textComponent(owner->GetComponent<bengine::TextComponent>()) {
    m_textComponent->SetText("Score: " + std::to_string(0));
}

void app::ScoreDisplayComponent::OnNotify(bengine::GameObject *gameObject, bengine::GameEvent event) {
    if (!m_pickupComponent) {
        m_pickupComponent = gameObject->GetComponent<PickupComponent>();
    }

    if (event == bengine::GameEvent::PickupPicked) {
        const int score = m_pickupComponent->GetScore();
        m_textComponent->SetText("Score: " + std::to_string(score));
    }
}
