#pragma once
#include "Patterns/Subject.h"
#include "Components/Component.h"

namespace app {
    class PickupComponent final : public bengine::Component, public bengine::Subject {
    public:
        explicit PickupComponent(bengine::GameObject *gameObject);

        void OnPickup(int value);
        [[nodiscard]] int GetScore() const;

    private:
        int m_score{0};
    };
}
