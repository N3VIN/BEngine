#include "GameSceneState.h"

#include "GameEndState.h"
#include "Gameplay/GameEvents.h"
#include "Commands/ChangeSceneCommand.h"
#include "Commands/MuteCommand.h"
#include "Level/BuildLevelScene.h"
#include "GameModes/GameModeFactory.h"
#include "GameModes/IGameMode.h"
#include "Score/ScoreBoard.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

std::unique_ptr<bengine::ISceneState> bomberman::MakeNextState(const std::vector<std::string> &levelPaths, size_t index, GameMode mode) {
    if (index + 1 < levelPaths.size()) {
        return std::make_unique<GameSceneState>(levelPaths, index + 1, mode);
    }

    return std::make_unique<GameEndState>();
}

bomberman::GameSceneState::GameSceneState(std::vector<std::string> levelPaths, size_t currentIndex, GameMode mode)
    : m_levelPaths(std::move(levelPaths))
  , m_currentIndex(currentIndex)
  , m_modeId(mode) {}

bomberman::GameSceneState::~GameSceneState() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::GameSceneState::OnEnter() {
    m_mode = CreateGameMode(m_modeId);

    if (m_currentIndex == 0) {
        scoreboard::Reset(m_mode->ShowsScore() ? m_mode->PlayerCount() : 0);
    }

    auto [scene, players] = BuildLevelScene(m_levelPaths[m_currentIndex], *m_mode);
    m_scene = &scene;
    m_players = std::move(players);
    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    bengine::InputManager::GetInstance().BindCommand(
        SDL_SCANCODE_F1, bengine::KeyState::Down,
        std::make_unique<ChangeSceneCommand>(
            [paths = m_levelPaths, index = m_currentIndex, mode = m_modeId] {
                return MakeNextState(paths, index, mode);
            }
        )
    );

    bengine::InputManager::GetInstance().BindCommand(
        SDL_SCANCODE_F2, bengine::KeyState::Down,
        std::make_unique<MuteCommand>()
    );

    m_playerDiedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::PlayerDied>(
        [this](const events::PlayerDied &event) {
            bengine::SceneManager::GetInstance().SetState(m_mode->MakeGameOverState(event.player, m_players));
        }
    );

    m_levelCompletedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::LevelCompleted>(
        [paths = m_levelPaths, index = m_currentIndex, mode = m_modeId](const events::LevelCompleted &) {
            bengine::SceneManager::GetInstance().SetState(MakeNextState(paths, index, mode));
        }
    );

    m_timeExpiredSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::TimeExpired>(
        [](const events::TimeExpired &) {
            bengine::SceneManager::GetInstance().SetState(std::make_unique<GameEndState>("TIME UP"));
        }
    );
}

void bomberman::GameSceneState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
