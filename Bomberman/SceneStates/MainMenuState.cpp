#include "MainMenuState.h"

#include "GameSceneState.h"
#include "../Commands/ChangeSceneCommand.h"
#include "../Level/LevelList.h"

#include "../../Core/SceneGraph/Scene.h"
#include "../../Core/SceneGraph/SceneManager.h"
#include "../../Core/SceneGraph/GameObject.h"
#include "../../Core/Components/TextComponent.h"
#include "../../Core/Components/RenderComponent.h"
#include "../../Core/Renderer/Renderer.h"
#include "../../Core/Renderer/ResourceManager.h"
#include "../../Core/Input/InputManager.h"

namespace dae {
    MainMenuState::MainMenuState() {
        Renderer::GetInstance().SetActiveCamera(nullptr);

        m_scene = &SceneManager::GetInstance().CreateScene();

        auto titleGo = std::make_unique<GameObject>();
        auto *titleText = titleGo->AddComponent<TextComponent>();
        titleText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
        titleText->SetColor({255, 255, 255, 255});
        titleText->SetText("BOMBERMAN");
        titleGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);

        const auto windowSize = Renderer::GetInstance().GetWindowSize();
        titleGo->SetLocalPosition(glm::vec2(windowSize.x * 0.5f - 180.0f, windowSize.y * 0.3f));
        m_scene->Add(std::move(titleGo));

        auto startGo = std::make_unique<GameObject>();
        auto *startText = startGo->AddComponent<TextComponent>();
        startText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
        startText->SetColor({200, 200, 200, 255});
        startText->SetText("Press F1 to switch scenes (for all scenes).");
        startGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);
        startGo->SetLocalPosition(glm::vec2(windowSize.x * 0.5f - 260.0f, windowSize.y * 0.5f));
        m_scene->Add(std::move(startGo));

        SceneManager::GetInstance().SetActiveScene(*m_scene);
    }

    MainMenuState::~MainMenuState() {
        if (m_scene) {
            SceneManager::GetInstance().DestroyScene(*m_scene);
        }
    }

    void MainMenuState::OnEnter() {
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
