#pragma once
#include "Components/Component.h"

namespace app {
    class HealthComponent final : public bengine::Component {
    public:
        explicit HealthComponent(bengine::GameObject *gameObject, int lives);

        void Die();
        [[nodiscard]] int GetLives() const;
        [[nodiscard]] bengine::GameObject *GetOwner() const;

    private:
        int m_lives;
    };
}
