#include "ScoreDisplayComponent.h"
#include "SceneGraph/GameObject.h"
#include "PickupComponent.h"
#include "Components/TextComponent.h"

namespace dae {
    ScoreDisplayComponent::~ScoreDisplayComponent() {
        if (m_pickupComponent) {
            m_pickupComponent->RemoveObserver(this);
        }
    }

    ScoreDisplayComponent::ScoreDisplayComponent(GameObject *owner)
        : Component(owner)
      , m_textComponent(owner->GetComponent<TextComponent>()) {
        m_textComponent->SetText("Score: " + std::to_string(0));
    }

    void ScoreDisplayComponent::OnNotify(GameObject *gameObject, GameEvent event) {
        if (!m_pickupComponent) {
            m_pickupComponent = gameObject->GetComponent<PickupComponent>();
        }

        if (event == GameEvent::PickupPicked) {
            const int score = m_pickupComponent->GetScore();
            m_textComponent->SetText("Score: " + std::to_string(score));
        }
    }
}
