#include "GameEndState.h"

#include "Commands/ChangeSceneCommand.h"
#include "HighScoreState.h"
#include "Score/ScoreBoard.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "UI/MenuUI.h"
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

    CreateMenuLabel(*m_scene, m_title, textSize::title, colors::danger, bengine::ScreenFraction(0.5f, 0.35f));

    const int playerCount = scoreboard::PlayerCount();
    for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex) {
        const auto label = (playerCount > 1) ? "P" + std::to_string(playerIndex + 1) + "  " : "SCORE  ";
        CreateMenuLabel(*m_scene, label + std::to_string(scoreboard::Get(playerIndex)), textSize::body, colors::white, bengine::ScreenFraction(0.5f, 0.45f + static_cast<float>(playerIndex) * 0.06f));
    }

    CreateMenuLabel(*m_scene, "ENTER continue", textSize::caption, colors::prompt, bengine::ScreenFraction(0.5f, 0.65f));

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    auto &input = bengine::InputManager::GetInstance();
    BindMuteToggle(input);

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
