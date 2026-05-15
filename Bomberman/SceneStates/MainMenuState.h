#pragma once
#include "SceneGraph/ISceneState.h"

namespace dae {
    class Scene;

    class MainMenuState final : public ISceneState {
    public:
        MainMenuState() = default;
        ~MainMenuState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        Scene *m_scene{};
    };
}
