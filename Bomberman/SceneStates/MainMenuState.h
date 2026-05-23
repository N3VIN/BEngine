#pragma once
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"

namespace bomberman {
    class MainMenuState final : public bengine::ISceneState {
    public:
        MainMenuState() = default;
        ~MainMenuState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        bengine::Scene *m_scene{};
    };
}
