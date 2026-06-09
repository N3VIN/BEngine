#pragma once
#include <string>
#include <vector>

#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"
#include "Patterns/MulticastDelegate.h"
#include "GameMode.h"

namespace bomberman {
    std::unique_ptr<bengine::ISceneState> MakeNextState(const std::vector<std::string> &levelPaths, size_t index, GameMode mode);

    class GameSceneState final : public bengine::ISceneState {
    public:
        GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex, GameMode mode);
        ~GameSceneState() override;

        void OnEnter() override;
        void OnExit() override;

        [[nodiscard]] GameMode GetMode() const { return m_mode; }

    private:
        std::vector<std::string> m_levelPaths;
        size_t m_currentIndex;
        GameMode m_mode;
        bengine::Scene *m_scene{};
        bengine::ScopedDelegate m_playerDiedSub;
        bengine::ScopedDelegate m_levelCompletedSub;
    };
}
