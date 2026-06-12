#include "MainMenuState.h"

#include "ModeSelectState.h"
#include "HighScoreState.h"
#include "MenuUI.h"
#include "Components/MenuComponent.h"
#include "Commands/QuitCommand.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/RenderComponent.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"
#include "Audio/IAudioService.h"
#include "Patterns/ServiceLocator.h"
#include "utils.h"

bomberman::MainMenuState::~MainMenuState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::MainMenuState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);

    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    auto &audio = bengine::ServiceLocator::GetAudioService();
    const auto &dataPath = bengine::ResourceManager::GetInstance().GetDataPath();
    audio.LoadAudio(utils::Hash("title_screen"), dataPath / "Audio/title_screen.wav");
    audio.StopAudio(utils::Hash("title_screen"));
    audio.PlayAudio(utils::Hash("title_screen"), 0.5f, -1);

    constexpr int logoX = 16;
    constexpr int logoY = 8;
    constexpr int logoWidth = 224;
    constexpr int logoHeight = 132;
    constexpr float logoScale = 1.85f;

    const auto windowSize = bengine::Renderer::GetInstance().GetWindowSize();
    auto *logoGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *logo = logoGo->AddComponent<bengine::RenderComponent>();
    logo->SetTexture("Sprites/Title_Text.png");
    logo->SetSourceRect(logoX, logoY, logoWidth, logoHeight);
    logo->SetScale(logoScale);
    logo->SetIgnoreCamera(true);
    logoGo->SetLocalPosition({(windowSize.x - static_cast<float>(logoWidth) * logoScale) * 0.5f, windowSize.y * 0.12f});

    auto *menuGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *menu = menuGo->AddComponent<MenuComponent>();

    constexpr float startY = 0.58f;
    constexpr float stepY = 0.09f;

    menu->AddItem(CreateMenuLabel(*m_scene, "Start", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY)),
                  [] {
                      bengine::SceneManager::GetInstance().SetState(std::make_unique<ModeSelectState>());
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Highscores", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY + stepY)),
                  [] {
                      bengine::SceneManager::GetInstance().SetState(std::make_unique<HighScoreState>());
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Quit", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY + stepY * 2.0f)),
                  [] {
                      QuitCommand{}.Execute();
                  }
    );

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    BindMenuInput(bengine::InputManager::GetInstance(), menu);
}

void bomberman::MainMenuState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
