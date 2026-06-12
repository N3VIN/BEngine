#include "MainMenuState.h"

#include "ModeSelectState.h"
#include "HighScoreState.h"
#include "UI/MenuUI.h"
#include "Colors.h"
#include "UI/UISizes.h"
#include "Components/Menu/MenuComponent.h"
#include "Commands/QuitCommand.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/RenderComponent.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"
#include "Audio/IAudioService.h"
#include "Patterns/ServiceLocator.h"
#include "Utils.h"

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
    audio.LoadAudio(bengine::Hash("title_screen"), dataPath / "Audio/title_screen.wav");
    audio.StopAudio(bengine::Hash("title_screen"));
    audio.PlayAudio(bengine::Hash("title_screen"), 0.5f, -1);

    static constexpr int logoX = 16;
    static constexpr int logoY = 8;
    static constexpr int logoWidth = 224;
    static constexpr int logoHeight = 132;
    static constexpr float logoScale = 1.85f;

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

    static constexpr float startY = 0.58f;
    static constexpr float stepY = 0.09f;

    menu->AddItem(CreateMenuLabel(*m_scene, "Start", textSize::body, bengine::colors::menuItem, bengine::ScreenFraction(0.5f, startY)),
                  [] {
                      bengine::SceneManager::GetInstance().SetState(std::make_unique<ModeSelectState>());
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Highscores", textSize::body, bengine::colors::menuItem, bengine::ScreenFraction(0.5f, startY + stepY)),
                  [] {
                      bengine::SceneManager::GetInstance().SetState(std::make_unique<HighScoreState>());
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Quit", textSize::body, bengine::colors::menuItem, bengine::ScreenFraction(0.5f, startY + stepY * 2.0f)),
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
