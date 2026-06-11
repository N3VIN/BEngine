#include "GameEndState.h"

#include "Commands/QuitCommand.h"
#include "Commands/ChangeSceneCommand.h"
#include "HighScoreState.h"
#include "ScoreBoard.h"

#include <string>

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "MenuUI.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"

bomberman::GameEndState::GameEndState(std::string title)
    : m_title(std::move(title)) {}

bomberman::GameEndState::~GameEndState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::GameEndState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);

    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    CreateMenuLabel(*m_scene, m_title, 48, {255, 80, 80, 255}, bengine::ScreenFraction(0.5f, 0.35f));

    const int playerCount = scoreboard::PlayerCount();
    for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex) {
        const auto label = (playerCount > 1) ? "P" + std::to_string(playerIndex + 1) + "  " : "SCORE  ";
        CreateMenuLabel(*m_scene, label + std::to_string(scoreboard::Get(playerIndex)), 28, {255, 255, 255, 255}, bengine::ScreenFraction(0.5f, 0.45f + static_cast<float>(playerIndex) * 0.06f));
    }

    CreateMenuLabel(*m_scene, "ENTER continue   ESC quit", 24, {200, 200, 200, 255}, bengine::ScreenFraction(0.5f, 0.65f));

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    auto &input = bengine::InputManager::GetInstance();
    input.BindCommand(SDL_SCANCODE_ESCAPE, bengine::KeyState::Down, std::make_unique<QuitCommand>());

    const auto toHighScores = [] {
        return std::make_unique<HighScoreState>(GetNewScores());
    };

    input.BindCommand(SDL_SCANCODE_RETURN, bengine::KeyState::Down, std::make_unique<ChangeSceneCommand>(toHighScores));
    input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, std::make_unique<ChangeSceneCommand>(toHighScores));
    input.BindCommand(0, bengine::Gamepad::Button::A, bengine::KeyState::Down, std::make_unique<ChangeSceneCommand>(toHighScores));
    input.BindCommand(0, bengine::Gamepad::Button::Start, bengine::KeyState::Down, std::make_unique<ChangeSceneCommand>(toHighScores));
}

void bomberman::GameEndState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}

void bomberman::GameEndState::Update(float deltaTime) {
    m_timer.Update(deltaTime);
    if (m_timer.IsExpired()) {
        bengine::SceneManager::GetInstance().SetState(std::make_unique<HighScoreState>(GetNewScores()));
    }
}
