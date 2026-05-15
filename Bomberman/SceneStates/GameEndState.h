#pragma once
#include "SceneGraph/ISceneState.h"

namespace dae {
    class Scene;

    class GameEndState final : public ISceneState {
    public:
        GameEndState() = default;
        ~GameEndState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        Scene *m_scene{};
    };
}
