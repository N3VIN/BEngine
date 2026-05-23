#pragma once
#include "Components/Component.h"
#include "SceneGraph/Scene.h"
#include "Patterns/MulticastDelegate.h"
#include "Timer.h"

namespace bomberman {
    class BrickComponent final : public bengine::Component {
    public:
        BrickComponent(bengine::GameObject *parent, bengine::Scene *scene, float destroyTime);
        ~BrickComponent() override = default;

        void Update(float deltaTime) override;
        void Destroy();

    private:
        bengine::Scene *m_scene;
        bengine::Timer m_timer;
        bool m_destroyed{false};
        bengine::ScopedDelegate m_destroySub;
    };
}
