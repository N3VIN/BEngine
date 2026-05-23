#include "MainMenuState.h"

#include "GameSceneState.h"
#include "Commands/ChangeSceneCommand.h"
#include "Level/LevelList.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/TextComponent.h"
#include "Components/RenderComponent.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"

bomberman::MainMenuState::~MainMenuState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::MainMenuState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);

    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    auto titleGo = std::make_unique<bengine::GameObject>();
    auto *titleText = titleGo->AddComponent<bengine::TextComponent>();
    titleText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
    titleText->SetColor({255, 255, 255, 255});
    titleText->SetText("BOMBERMAN");
    titleGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);

    titleGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.3f) + glm::vec2(-180.0f, 0.0f));
    m_scene->Add(std::move(titleGo));

    auto startGo = std::make_unique<bengine::GameObject>();
    auto *startText = startGo->AddComponent<bengine::TextComponent>();
    startText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
    startText->SetColor({200, 200, 200, 255});
    startText->SetText("Press F1 to switch scenes (for all scenes).");
    startGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
    startGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.5f) + glm::vec2(-260.0f, 0.0f));
    m_scene->Add(std::move(startGo));

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    bengine::InputManager::GetInstance().BindCommand(
        SDL_SCANCODE_F1, bengine::KeyState::Down,
        std::make_unique<ChangeSceneCommand>([] {
                return std::make_unique<GameSceneState>(GetLevelList(), 0);
            }
        )
    );
}

void bomberman::MainMenuState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
