#pragma once
#include "Components/Component.h"
#include "Components/TextComponent.h"
#include "Patterns/MulticastDelegate.h"

namespace app {
    class HealthComponent;

    class LivesDisplayComponent final : public bengine::Component {
    public:
        ~LivesDisplayComponent() override = default;
        explicit LivesDisplayComponent(bengine::GameObject *owner, HealthComponent *healthComponent);

    private:
        bengine::TextComponent *m_textComponent;
        HealthComponent *m_healthComponent{nullptr};
        bengine::ScopedDelegate m_damageSD;
    };
}
