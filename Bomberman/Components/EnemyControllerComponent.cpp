#include "EnemyControllerComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "HealthComponent.h"
#include "States/EnemyStates.h"
#include "GameEvents.h"
#include "Level/Tileset.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"

bomberman::EnemyControllerComponent::EnemyControllerComponent(bengine::GameObject *parent, bengine::Scene *scene, EnemyType type)
    : bengine::Component(parent)
  , m_scene(scene)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<SpriteRendererComponent>())
  , m_health(parent->GetComponent<HealthComponent>())
  , m_sprites(&GetTileset().GetEnemySprites(type))
  , m_points(GetEnemyStats(type).points) {
    m_currentState = MakeEnemyWalkState(m_movement->GetFacing());
    m_currentState->OnEnter(*this);

    m_damagedSub = m_health->SubscribeDamaged(
        [this](int) {
            m_killed = true;
        }
    );
}

void bomberman::EnemyControllerComponent::Update(float deltaTime) {
    if (m_killed) {
        m_killed = false;
        if (m_currentState->IsAlive()) {
            m_currentState->OnExit(*this);
            m_currentState = std::make_unique<EnemyDyingState>();
            m_currentState->OnEnter(*this);
            return;
        }
    }

    if (auto next = m_currentState->Update(*this, deltaTime)) {
        m_currentState->OnExit(*this);
        m_currentState = std::move(next);
        m_currentState->OnEnter(*this);
    }
}

bool bomberman::EnemyControllerComponent::IsAlive() const {
    return m_currentState->IsAlive();
}

glm::ivec2 bomberman::EnemyControllerComponent::GetFacing() const {
    return m_movement->GetFacing();
}

bool bomberman::EnemyControllerComponent::IsAnimationPlaying() const {
    return m_sprite->IsPlaying();
}

void bomberman::EnemyControllerComponent::PlayWalkAnimation(bool facingLeft) const {
    m_sprite->Play(facingLeft ? m_sprites->walkLeft : m_sprites->walkRight, true);
}

void bomberman::EnemyControllerComponent::PlayDeathAnimation() const {
    m_sprite->Play(m_sprites->death, false);
}

void bomberman::EnemyControllerComponent::Die() const {
    bengine::ServiceLocator::GetEventBus().Broadcast(events::EnemyKilled{
            .enemy = GetParent(),
            .cell = m_movement->GetCell(),
            .points = m_points,
        }
    );

    m_scene->Remove(GetParent());
}
