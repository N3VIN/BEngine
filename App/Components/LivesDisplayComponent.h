#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace dae {
    class TextComponent;
    class HealthComponent;

    class LivesDisplayComponent final : public Component {
    public:
        ~LivesDisplayComponent() override = default;
        explicit LivesDisplayComponent(GameObject *owner, HealthComponent *healthComponent);

    private:
        TextComponent *m_textComponent;
        HealthComponent *m_healthComponent{nullptr};
        ScopedDelegate m_damageSD;
    };
}
