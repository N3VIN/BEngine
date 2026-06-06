#include "GameSceneState.h"

#include "GameEndState.h"
#include "GameEvents.h"
#include "Commands/ChangeSceneCommand.h"
#include "Level/BuildLevelScene.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

std::unique_ptr<bengine::ISceneState> bomberman::MakeNextState(const std::vector<std::string> &levelPaths, size_t index) {
    if (index + 1 < levelPaths.size()) {
        return std::make_unique<GameSceneState>(levelPaths, index + 1);
    }

    return std::make_unique<GameEndState>();
}

bomberman::GameSceneState::GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex)
    : m_levelPaths(std::move(levelPaths))
  , m_currentIndex(currentIndex) {}

bomberman::GameSceneState::~GameSceneState() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::GameSceneState::OnEnter() {
    m_scene = &BuildLevelScene(m_levelPaths[m_currentIndex]);
    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    bengine::InputManager::GetInstance().BindCommand(
        SDL_SCANCODE_F1, bengine::KeyState::Down,
        std::make_unique<ChangeSceneCommand>(
            [paths = m_levelPaths, index = m_currentIndex] {
                return MakeNextState(paths, index);
            }
        )
    );

    m_playerDiedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::PlayerDied>(
        [](const events::PlayerDied &) {
            bengine::SceneManager::GetInstance().SetState(std::make_unique<GameEndState>());
        }
    );

    m_levelCompletedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::LevelCompleted>(
        [paths = m_levelPaths, index = m_currentIndex](const events::LevelCompleted &) {
            bengine::SceneManager::GetInstance().SetState(MakeNextState(paths, index));
        }
    );
}

void bomberman::GameSceneState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
