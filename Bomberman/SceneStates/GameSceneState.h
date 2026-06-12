#pragma once
#include <memory>
#include <string>
#include <vector>

#include "SceneGraph/ISceneState.h"
#include "SceneGraph/Scene.h"
#include "Patterns/MulticastDelegate.h"
#include "GameMode.h"

namespace bengine {
    class GameObject;
}

namespace bomberman {
    class IGameMode;

    std::unique_ptr<bengine::ISceneState> MakeNextState(const std::vector<std::string> &levelPaths, size_t index, GameMode mode);

    class GameSceneState final : public bengine::ISceneState {
    public:
        GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex, GameMode mode);
        ~GameSceneState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        std::vector<std::string> m_levelPaths;
        size_t m_currentIndex;
        GameMode m_modeId;
        std::unique_ptr<IGameMode> m_mode{};
        bengine::Scene *m_scene{};
        std::vector<bengine::GameObject *> m_players{};
        bengine::ScopedDelegate m_playerDiedSub;
        bengine::ScopedDelegate m_levelCompletedSub;
        bengine::ScopedDelegate m_timeExpiredSub;
    };
}
