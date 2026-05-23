#pragma once
#include "Patterns/IObserver.h"
#include "Components/Component.h"
#include "Components/TextComponent.h"


namespace app {
    class PickupComponent;

    class ScoreDisplayComponent final : public bengine::Component, public bengine::IObserver {
    public:
        ~ScoreDisplayComponent() override;
        explicit ScoreDisplayComponent(bengine::GameObject *owner);
        void OnNotify(bengine::GameObject *gameObject, bengine::GameEvent event) override;

    private:
        bengine::TextComponent *m_textComponent;
        PickupComponent *m_pickupComponent{nullptr};
    };
}
