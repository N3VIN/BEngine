#include "MainMenuState.h"

#include "ModeSelectState.h"
#include "HighScoreState.h"
#include "MenuUI.h"
#include "Components/MenuComponent.h"
#include "Commands/MenuNavigateCommand.h"
#include "Commands/MenuConfirmCommand.h"
#include "Commands/QuitCommand.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"
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

    CreateMenuLabel(*m_scene, "BOMBERMAN", 48, {255, 255, 255, 255}, bengine::ScreenFraction(0.5f, 0.25f));

    auto *menuGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *menu = menuGo->AddComponent<MenuComponent>();

    constexpr float startY = 0.5f;
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

    auto &input = bengine::InputManager::GetInstance();
    input.BindCommand(SDL_SCANCODE_UP, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, -1));
    input.BindCommand(SDL_SCANCODE_DOWN, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, 1));
    input.BindCommand(SDL_SCANCODE_RETURN, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
    input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));

    input.BindCommand(0, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, -1));
    input.BindCommand(0, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, 1));
    input.BindCommand(0, bengine::Gamepad::Button::A, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
    input.BindCommand(0, bengine::Gamepad::Button::Start, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
}

void bomberman::MainMenuState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
