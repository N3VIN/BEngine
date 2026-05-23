#include "BrickComponent.h"
#include "SpriteRendererComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"

bomberman::BrickComponent::BrickComponent(bengine::GameObject *parent, bengine::Scene *scene, float destroyTime)
    : bengine::Component(parent)
  , m_scene(scene)
  , m_timer(destroyTime) {
    m_destroySub = bengine::ServiceLocator::GetEventBus().Subscribe<events::BrickDestroyed>(
        [this, parent](const events::BrickDestroyed &event) {
            if (event.brick == parent) {
                Destroy();
            }
        }
    );
}

void bomberman::BrickComponent::Update(float deltaTime) {
    if (!m_destroyed) {
        return;
    }

    m_timer.Update(deltaTime);
    if (m_timer.IsExpired()) {
        m_scene->Remove(GetParent());
    }
}

void bomberman::BrickComponent::Destroy() {
    if (m_destroyed) {
        return;
    }

    if (auto *sprite = GetParent()->GetComponent<SpriteRendererComponent>()) {
        sprite->PlayOnce(SpriteRendererComponent::ANIMATION_FPS);
    }

    m_timer.Reset();
    m_destroyed = true;
}
