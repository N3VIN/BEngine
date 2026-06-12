#include "EnemyControllerComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "States/EnemyStates.h"
#include "Gameplay/GameEvents.h"
#include "Level/Tileset.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"

bomberman::EnemyControllerComponent::EnemyControllerComponent(bengine::GameObject *parent, EnemyType type)
    : bengine::Component(parent)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<bengine::SpriteRendererComponent>())
  , m_health(parent->GetComponent<HealthComponent>())
  , m_sprites(&GetTileset().GetEnemySprites(type))
  , m_stats(&GetEnemyStats(type)) {
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
    m_sprite->Play(facingLeft ? m_sprites->walkLeft : m_sprites->walkRight, true, GetTileset().animationFps);
}

void bomberman::EnemyControllerComponent::PlayDeathAnimation() const {
    m_sprite->Play(m_sprites->death, false, GetTileset().animationFps);
}

void bomberman::EnemyControllerComponent::StopMovement() const {
    m_movement->SetMovementEnabled(false);
}

void bomberman::EnemyControllerComponent::Die() const {
    bengine::ServiceLocator::GetEventBus().Broadcast(events::EnemyKilled{
            .enemy = GetParent(),
            .killer = m_health->GetLastAttacker(),
            .cell = m_movement->GetCell(),
            .points = m_stats->points,
        }
    );

    bengine::GetActiveScene()->Remove(GetParent());
}
