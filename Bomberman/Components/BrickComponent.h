#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Timer.h"

namespace dae {
    class Scene;

    class BrickComponent final : public Component {
    public:
        BrickComponent(GameObject *parent, Scene *scene, float destroyTime);
        ~BrickComponent() override = default;

        void Update(float deltaTime) override;
        void Destroy();

    private:
        Scene *m_scene;
        Timer m_timer;
        bool m_destroyed{false};
        ScopedDelegate m_destroySub;
    };
}
