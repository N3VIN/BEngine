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

namespace dae {
    MainMenuState::~MainMenuState() {
        if (m_scene) {
            SceneManager::GetInstance().DestroyScene(*m_scene);
        }
    }

    void MainMenuState::OnEnter() {
        Renderer::GetInstance().SetActiveCamera(nullptr);

        m_scene = &SceneManager::GetInstance().CreateScene();

        auto titleGo = std::make_unique<GameObject>();
        auto *titleText = titleGo->AddComponent<TextComponent>();
        titleText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
        titleText->SetColor({255, 255, 255, 255});
        titleText->SetText("BOMBERMAN");
        titleGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);

        titleGo->SetLocalPosition(ScreenFraction(0.5f, 0.3f) + glm::vec2(-180.0f, 0.0f));
        m_scene->Add(std::move(titleGo));

        auto startGo = std::make_unique<GameObject>();
        auto *startText = startGo->AddComponent<TextComponent>();
        startText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
        startText->SetColor({200, 200, 200, 255});
        startText->SetText("Press F1 to switch scenes (for all scenes).");
        startGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);
        startGo->SetLocalPosition(ScreenFraction(0.5f, 0.5f) + glm::vec2(-260.0f, 0.0f));
        m_scene->Add(std::move(startGo));

        SceneManager::GetInstance().SetActiveScene(*m_scene);

        InputManager::GetInstance().BindCommand(
            SDL_SCANCODE_F1, KeyState::Down,
            std::make_unique<ChangeSceneCommand>([] {
                    return std::make_unique<GameSceneState>(GetLevelList(), 0);
                }
            )
        );
    }

    void MainMenuState::OnExit() {
        InputManager::GetInstance().UnbindAll();
    }
}
