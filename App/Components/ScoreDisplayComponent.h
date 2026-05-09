#pragma once
#include "Patterns/IObserver.h"
#include "Components/Component.h"


namespace dae {
    class TextComponent;
    class PickupComponent;

    class ScoreDisplayComponent final : public Component, public IObserver {
    public:
        ~ScoreDisplayComponent() override;
        explicit ScoreDisplayComponent(GameObject *owner);
        void OnNotify(GameObject *gameObject, GameEvent event) override;

    private:
        TextComponent *m_textComponent;
        PickupComponent *m_pickupComponent{nullptr};
    };
}
