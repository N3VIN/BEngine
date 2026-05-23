#pragma once
#include "Component.h"

namespace bengine {
    class RotatorComponent final : public Component {
    public:
        explicit RotatorComponent(GameObject *parent);

        void SetSpeed(float period, bool isClockwise);
        void SetRadius(float radius);

        void Update(float deltaTime) override;

    private:
        float m_speed{};
        float m_radius{};
        float m_angle{};
    };
}
