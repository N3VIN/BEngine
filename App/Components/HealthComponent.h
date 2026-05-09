#pragma once
#include "Components/Component.h"
// #include "Patterns/Subject.h"
#include "Patterns/MulticastDelegate.h"

namespace dae {
    class HealthComponent final : public Component {
    public:
        explicit HealthComponent(GameObject *gameObject, int lives);

        MulticastDelegate<int> OnLifeChanged;

        void Die();
        [[nodiscard]] int GetLives() const;

    private:
        int m_lives;
    };
}
