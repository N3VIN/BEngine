#pragma once
#include <string>
#include <vector>

#include "SceneGraph/ISceneState.h"

namespace dae {
    class Scene;

    class GameSceneState final : public ISceneState {
    public:
        GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex);
        ~GameSceneState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        std::vector<std::string> m_levelPaths;
        size_t m_currentIndex; // current scene index
        Scene *m_scene{};
    };
}
