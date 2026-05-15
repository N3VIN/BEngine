#pragma once
#include "Components/Component.h"

namespace dae {
    class HealthComponent final : public Component {
    public:
        explicit HealthComponent(GameObject *gameObject, int lives);

        void Die();
        [[nodiscard]] int GetLives() const;
        [[nodiscard]] GameObject *GetOwner() const;

    private:
        int m_lives;
    };
}
