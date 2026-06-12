#pragma once
#include "Components/Component.h"
#include "Timer.h"

namespace bomberman {
    class BrickComponent final : public bengine::Component {
    public:
        BrickComponent(bengine::GameObject *parent, float destroyTime);
        ~BrickComponent() override = default;

        void Update(float deltaTime) override;
        void Destroy();

    private:
        bengine::Timer m_timer;
        bool m_destroyed{false};
    };
}
