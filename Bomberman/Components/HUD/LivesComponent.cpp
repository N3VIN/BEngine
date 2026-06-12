#include "LivesComponent.h"
#include "Components/HealthComponent.h"
#include "SceneGraph/GameObject.h"
#include "Components/SpriteTextComponent.h"

bomberman::LivesComponent::LivesComponent(bengine::GameObject *owner, bengine::GameObject *player)
    : bengine::Component(owner)
  , m_text(owner->GetComponent<bengine::SpriteTextComponent>()) {
    auto *health = player->GetComponent<HealthComponent>();
    Refresh(health->GetLives());

    m_damagedSub = health->SubscribeDamaged([this](int lives) {
            Refresh(lives);
        }
    );
}

void bomberman::LivesComponent::Refresh(int lives) const {
    m_text->SetText("LEFT " + std::to_string(lives));
}
