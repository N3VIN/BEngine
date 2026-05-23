#include "GameEndState.h"

#include "Commands/QuitCommand.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/TextComponent.h"
#include "Components/RenderComponent.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"

bomberman::GameEndState::~GameEndState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::GameEndState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);

    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    auto titleGo = std::make_unique<bengine::GameObject>();
    auto *titleText = titleGo->AddComponent<bengine::TextComponent>();
    titleText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48));
    titleText->SetColor({255, 80, 80, 255});
    titleText->SetText("GAME OVER");
    titleGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);

    titleGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.35f) + glm::vec2(-170.0f, 0.0f));
    m_scene->Add(std::move(titleGo));

    auto quitGo = std::make_unique<bengine::GameObject>();
    auto *quitText = quitGo->AddComponent<bengine::TextComponent>();
    quitText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
    quitText->SetColor({200, 200, 200, 255});
    quitText->SetText("Press ESC to quit");
    quitGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
    quitGo->SetLocalPosition(bengine::ScreenFraction(0.5f, 0.55f) + glm::vec2(-110.0f, 0.0f));
    m_scene->Add(std::move(quitGo));

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);

    bengine::InputManager::GetInstance().BindCommand(
        SDL_SCANCODE_ESCAPE, bengine::KeyState::Down,
        std::make_unique<QuitCommand>()
    );
}

void bomberman::GameEndState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}
