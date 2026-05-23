#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace bomberman {
    class GridMovementComponent;

    class HealthComponent final : public bengine::Component {
    public:
        HealthComponent(bengine::GameObject *parent, int lives);
        ~HealthComponent() override = default;

        void TakeDamage(int amount);
        [[nodiscard]] int GetLives() const;

    private:
        int m_lives;
        bengine::ScopedDelegate m_explosionSub;
    };
}
