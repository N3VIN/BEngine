#include "GameSceneState.h"

#include "GameEndState.h"
#include "Commands/ChangeSceneCommand.h"
#include "Level/BuildLevelScene.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"

namespace dae {
    std::unique_ptr<ISceneState> MakeNextState(const std::vector<std::string> &levelPaths, size_t index) {
        if (index + 1 < levelPaths.size()) {
            return std::make_unique<GameSceneState>(std::move(levelPaths), index + 1); // if there are multiple levels
        }

        return std::make_unique<GameEndState>();
    }

    GameSceneState::GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex)
        : m_levelPaths(std::move(levelPaths))
      , m_currentIndex(currentIndex) {}

    GameSceneState::~GameSceneState() {
        Renderer::GetInstance().SetActiveCamera(nullptr);
        if (m_scene) {
            SceneManager::GetInstance().DestroyScene(*m_scene);
        }
    }

    void GameSceneState::OnEnter() {
        m_scene = &BuildLevelScene(m_levelPaths[m_currentIndex]); // we do this here instead of the constructor since the change scene command execute fn runs before the UnbindAll for the prev scene
        SceneManager::GetInstance().SetActiveScene(*m_scene);

        InputManager::GetInstance().BindCommand(
            SDL_SCANCODE_F1, KeyState::Down,
            std::make_unique<ChangeSceneCommand>(
                [paths = m_levelPaths, index = m_currentIndex] {
                    return MakeNextState(paths, index);
                }
            )
        );
    }

    void GameSceneState::OnExit() {
        InputManager::GetInstance().UnbindAll();
    }
}
