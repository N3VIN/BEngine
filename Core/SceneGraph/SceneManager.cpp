#include "SceneManager.h"
#include "Scene.h"

void bengine::SceneManager::Update(float deltaTime) {
    if (m_pendingState) { // when there is a scene transition
        if (m_currentState) {
            m_currentState->OnExit();
        }

        m_currentState.reset();
        m_currentState = std::move(m_pendingState);
        m_currentState->OnEnter();
    }

    if (m_currentState) {
        m_currentState->Update(deltaTime);
    }

    if (m_activeScene) {
        m_activeScene->Update(deltaTime);
    }
}

void bengine::SceneManager::FixedUpdate() {
    if (m_activeScene) {
        m_activeScene->FixedUpdate();
    }
}

void bengine::SceneManager::Render() const {
    if (m_activeScene) {
        m_activeScene->Render();
    }
}

bengine::Scene &bengine::SceneManager::CreateScene() {
    m_scenes.emplace_back(new Scene());
    auto &scene = *m_scenes.back();
    if (!m_activeScene) {
        m_activeScene = &scene;
    }

    return scene;
}

void bengine::SceneManager::DestroyScene(Scene &scene) {
    if (m_activeScene == &scene) {
        m_activeScene = nullptr;
    }

    std::erase_if(m_scenes, [&scene](const std::unique_ptr<Scene> &s) {
                      return s.get() == &scene;
                  }
    );
}

void bengine::SceneManager::SetActiveScene(Scene &scene) {
    m_activeScene = &scene;
}

void bengine::SceneManager::SetState(std::unique_ptr<ISceneState> state) {
    m_pendingState = std::move(state);
}
