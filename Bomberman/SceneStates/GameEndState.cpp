#include "GameEndState.h"

#include "Commands/QuitCommand.h"
#include "Commands/ChangeSceneCommand.h"
#include "HighScoreState.h"
#include "ScoreBoard.h"

#include <string>

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/TextComponent.h"
#include "Components/RenderComponent.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
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

    auto *titleGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *titleText = titleGo->AddComponent<bengine::TextComponent>();
    titleText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
    titleText->SetColor({255, 80, 80, 255});
    titleText->SetText(m_title);
    titleGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);

    titleGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.35f) + glm::vec2(-170.0f, 0.0f));

    const int playerCount = scoreboard::PlayerCount();
    for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex) {
        auto *scoreGo = m_scene->Add(std::make_unique<bengine::GameObject>());
        auto *scoreText = scoreGo->AddComponent<bengine::TextComponent>();
        scoreText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28));
        scoreText->SetColor({255, 255, 255, 255});

        const auto label = (playerCount > 1) ? "P" + std::to_string(playerIndex + 1) + "  " : "SCORE  ";
        scoreText->SetText(label + std::to_string(scoreboard::Get(playerIndex)));
        scoreGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
        scoreGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.45f + static_cast<float>(playerIndex) * 0.06f) + glm::vec2(-90.0f, 0.0f));
    }

    auto *quitGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *quitText = quitGo->AddComponent<bengine::TextComponent>();
    quitText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
    quitText->SetColor({200, 200, 200, 255});
    quitText->SetText("ENTER continue   ESC quit");
    quitGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
    quitGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.65f) + glm::vec2(-150.0f, 0.0f));

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    auto &input = bengine::InputManager::GetInstance();
    input.BindCommand(SDL_SCANCODE_ESCAPE, bengine::KeyState::Down, std::make_unique<QuitCommand>());

    const auto toHighScores = [] { return std::make_unique<HighScoreState>(GetNewScores()); };
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
