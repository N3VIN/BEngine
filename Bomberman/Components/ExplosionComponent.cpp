#include "ExplosionComponent.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"

bomberman::ExplosionComponent::ExplosionComponent(bengine::GameObject *parent, bengine::Scene *scene, float lifetime)
    : bengine::Component(parent)
  , m_scene(scene)
  , m_timer(lifetime) {}

void bomberman::ExplosionComponent::Update(float deltaTime) {
    m_timer.Update(deltaTime);
    if (m_timer.IsExpired() && !m_removed) {
        m_removed = true;
        m_scene->Remove(GetParent());
    }
}
