#pragma once
#include <string>
#include <vector>

#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"
#include "Patterns/MulticastDelegate.h"

namespace bomberman {
    std::unique_ptr<bengine::ISceneState> MakeNextState(const std::vector<std::string> &levelPaths, size_t index);

    class GameSceneState final : public bengine::ISceneState {
    public:
        GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex);
        ~GameSceneState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        std::vector<std::string> m_levelPaths;
        size_t m_currentIndex;
        bengine::Scene *m_scene{};
        bengine::ScopedDelegate m_playerDiedSub;
        bengine::ScopedDelegate m_levelCompletedSub;
    };
}
