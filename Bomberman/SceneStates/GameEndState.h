#pragma once
#include <string>
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"

namespace bomberman {
    class GameEndState final : public bengine::ISceneState {
    public:
        explicit GameEndState(std::string title = "GAME OVER");
        ~GameEndState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        std::string m_title;
        bengine::Scene *m_scene{};
    };
}
