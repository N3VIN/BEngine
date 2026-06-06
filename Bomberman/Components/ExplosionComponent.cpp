#include "ExplosionComponent.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"

bomberman::ExplosionComponent::ExplosionComponent(bengine::GameObject *parent, float lifetime)
    : bengine::Component(parent)
  , m_timer(lifetime) {}

void bomberman::ExplosionComponent::Update(float deltaTime) {
    m_timer.Update(deltaTime);
    if (m_timer.IsExpired() && !m_removed) {
        m_removed = true;
        bengine::GetActiveScene()->Remove(GetParent());
    }
}
