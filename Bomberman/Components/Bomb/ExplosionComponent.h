#pragma once
#include "Components/Component.h"
#include "Timer.h"

namespace bomberman {
    class ExplosionComponent final : public bengine::Component {
    public:
        ExplosionComponent(bengine::GameObject *parent, float lifetime);
        ~ExplosionComponent() override = default;

        void Update(float deltaTime) override;

    private:
        bengine::Timer m_timer;
        bool m_removed{false};
    };
}
