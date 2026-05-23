#pragma once
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"

namespace bomberman {
    class GameEndState final : public bengine::ISceneState {
    public:
        GameEndState() = default;
        ~GameEndState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        bengine::Scene *m_scene{};
    };
}
