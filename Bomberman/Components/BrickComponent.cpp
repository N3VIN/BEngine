#include "BrickComponent.h"
#include "SpriteRendererComponent.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"

bomberman::BrickComponent::BrickComponent(bengine::GameObject *parent, float destroyTime)
    : bengine::Component(parent)
  , m_timer(destroyTime) {}

void bomberman::BrickComponent::Update(float deltaTime) {
    if (!m_destroyed) {
        return;
    }

    m_timer.Update(deltaTime);
    if (m_timer.IsExpired()) {
        bengine::GetActiveScene()->Remove(GetParent());
    }
}

void bomberman::BrickComponent::Destroy() {
    if (m_destroyed) {
        return;
    }

    if (auto *sprite = GetParent()->GetComponent<SpriteRendererComponent>()) {
        sprite->PlayOnce(GetTileset().animationFps);
    }

    m_timer.Reset();
    m_destroyed = true;
}
