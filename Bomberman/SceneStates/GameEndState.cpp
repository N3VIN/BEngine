#include "GameEndState.h"

#include "Commands/QuitCommand.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/TextComponent.h"
#include "Components/RenderComponent.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"

namespace dae {
    GameEndState::GameEndState() {
        Renderer::GetInstance().SetActiveCamera(nullptr);

        m_scene = &SceneManager::GetInstance().CreateScene();

        auto titleGo = std::make_unique<GameObject>();
        auto *titleText = titleGo->AddComponent<TextComponent>();
        titleText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
        titleText->SetColor({255, 80, 80, 255});
        titleText->SetText("GAME OVER");
        titleGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);

        const auto windowSize = Renderer::GetInstance().GetWindowSize();
        titleGo->SetLocalPosition(glm::vec2(windowSize.x * 0.5f - 170.0f, windowSize.y * 0.35f));
        m_scene->Add(std::move(titleGo));

        auto quitGo = std::make_unique<GameObject>();
        auto *quitText = quitGo->AddComponent<TextComponent>();
        quitText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
        quitText->SetColor({200, 200, 200, 255});
        quitText->SetText("Press ESC to quit");
        quitGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);
        quitGo->SetLocalPosition(glm::vec2(windowSize.x * 0.5f - 110.0f, windowSize.y * 0.55f));
        m_scene->Add(std::move(quitGo));

        SceneManager::GetInstance().SetActiveScene(*m_scene);
    }

    GameEndState::~GameEndState() {
        if (m_scene) {
            SceneManager::GetInstance().DestroyScene(*m_scene);
        }
    }

    void GameEndState::OnEnter() {
        InputManager::GetInstance().BindCommand(
            SDL_SCANCODE_ESCAPE, KeyState::Down,
            std::make_unique<QuitCommand>()
        );
    }

    void GameEndState::OnExit() {
        InputManager::GetInstance().UnbindAll();
    }
}
