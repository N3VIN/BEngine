#pragma once
#include "../../Core/Components/Component.h"
#include "../../Core/Patterns/MulticastDelegate.h"

namespace dae {
    class HealthComponent final : public Component { // we have a similar component in the App but I think that will be removed so a similar copy
    public:
        HealthComponent(GameObject *parent, int lives);

        MulticastDelegate<int> OnLifeChanged;

        void TakeDamage(int amount);
        [[nodiscard]] int GetLives() const;

    private:
        int m_lives;
    };
}
