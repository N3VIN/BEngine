#pragma once
#include <string>
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"
#include "Timer.h"

namespace bomberman {
    class GameEndState final : public bengine::ISceneState {
    public:
        explicit GameEndState(std::string title = "GAME OVER");
        ~GameEndState() override;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;

    private:
        std::string m_title;
        bengine::Scene *m_scene{};
        bengine::Timer m_timer{3.0f};
    };
}
