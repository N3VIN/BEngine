#pragma once
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"

namespace bomberman {
    class ModeSelectState final : public bengine::ISceneState {
    public:
        ModeSelectState() = default;
        ~ModeSelectState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        bengine::Scene *m_scene{};
    };
}
