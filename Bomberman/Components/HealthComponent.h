#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace dae {
    class GridMovementComponent;

    class HealthComponent final : public Component {
    public:
        HealthComponent(GameObject *parent, int lives);
        ~HealthComponent() override = default;

        void TakeDamage(int amount);
        [[nodiscard]] int GetLives() const;

    private:
        int m_lives;
        ScopedDelegate m_explosionSub;
    };
}
