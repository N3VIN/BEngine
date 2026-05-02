#include "LivesDisplayComponent.h"
#include "../../Core/SceneGraph/GameObject.h"
#include "HealthComponent.h"
#include "Components/TextComponent.h"

namespace dae {
    LivesDisplayComponent::LivesDisplayComponent(GameObject *owner, HealthComponent *healthComponent)
        : Component(owner)
      , m_textComponent(owner->GetComponent<TextComponent>())
      , m_healthComponent(healthComponent) {
        if (m_healthComponent) {
            const auto life = healthComponent->GetLives();
            m_textComponent->SetText("Score: " + std::to_string(life));

            // Lambda subscription
            // m_lambdaHandle = m_healthComponent->OnLifeChanged.Subscribe([this](int lives) {
            //         m_textComponent->SetText("Lives: " + std::to_string(lives));
            //     }
            // );

            // member function subscription
            m_lambdaSD = m_healthComponent->OnLifeChanged.Subscribe(this, &LivesDisplayComponent::OnLifeChanged);
        }
    }

    void LivesDisplayComponent::OnLifeChanged(int lives) const {
        m_textComponent->SetText("Lives: " + std::to_string(lives));
    }

}
