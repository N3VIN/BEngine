#include "ModeSelectState.h"

#include "MainMenuState.h"
#include "GameSceneState.h"
#include "GameMode.h"
#include "MenuUI.h"
#include "Level/LevelList.h"
#include "Components/MenuComponent.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Audio/IAudioService.h"
#include "Patterns/ServiceLocator.h"
#include "utils.h"

bomberman::ModeSelectState::~ModeSelectState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::ModeSelectState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);

    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    CreateMenuLabel(*m_scene, "SELECT MODE", 48, {255, 255, 255, 255}, bengine::ScreenFraction(0.5f, 0.25f));

    auto *menuGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    auto *menu = menuGo->AddComponent<MenuComponent>();

    const auto launch = [](GameMode mode) {
        bengine::ServiceLocator::GetAudioService().StopAudio(utils::Hash("title_screen"));
        bengine::SceneManager::GetInstance().SetState(std::make_unique<GameSceneState>(GetLevelList(), 0, mode));
    };

    constexpr float startY = 0.45f;
    constexpr float stepY = 0.09f;

    menu->AddItem(CreateMenuLabel(*m_scene, "Solo", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY)),
                  [launch] {
                      launch(GameMode::Solo);
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "COOP", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY + stepY)),
                  [launch] {
                      launch(GameMode::Coop);
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Versus", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY + stepY * 2.0f)),
                  [launch] {
                      launch(GameMode::Versus);
                  }
    );

    menu->AddItem(CreateMenuLabel(*m_scene, "Back", 28, {180, 180, 180, 255}, bengine::ScreenFraction(0.5f, startY + stepY * 3.0f)),
                  [] {
                      bengine::SceneManager::GetInstance().SetState(std::make_unique<MainMenuState>());
                  }
    );

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    BindMenuInput(bengine::InputManager::GetInstance(), menu);
}

void bomberman::ModeSelectState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
