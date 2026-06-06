#pragma once
#include "Components/Component.h"
#include "Timer.h"

namespace bengine {
    class Scene;
}

namespace bomberman {
    class ExplosionComponent final : public bengine::Component {
    public:
        ExplosionComponent(bengine::GameObject *parent, bengine::Scene *scene, float lifetime);
        ~ExplosionComponent() override = default;

        void Update(float deltaTime) override;

    private:
        bengine::Scene *m_scene;
        bengine::Timer m_timer;
        bool m_removed{false};
    };
}
