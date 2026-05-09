#pragma once
#include "Patterns/Subject.h"
#include "Components/Component.h"

namespace dae {
    class PickupComponent final : public Component, public Subject {
    public:
        explicit PickupComponent(GameObject *gameObject);

        void OnPickup(int value);
        [[nodiscard]] int GetScore() const;

    private:
        int m_score{0};
    };
}
