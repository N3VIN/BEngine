#pragma once
#include "../../Core/Patterns/ISceneState.h"

namespace dae {
    class Scene;

    class MainMenuState final : public ISceneState {
    public:
        MainMenuState();
        ~MainMenuState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        Scene *m_scene{};
    };
}
