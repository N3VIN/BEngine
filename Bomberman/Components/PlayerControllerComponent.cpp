#include "PlayerControllerComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "HealthComponent.h"
#include "GameEvents.h"
#include "States/PlayerStates.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"

bomberman::PlayerControllerComponent::PlayerControllerComponent(bengine::GameObject *parent)
    : bengine::Component(parent)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<SpriteRendererComponent>())
  , m_health(parent->GetComponent<HealthComponent>())
  , m_spawnCell(m_movement->GetCell()) {
    m_currentState = std::make_unique<IdlePlayerState>();
    m_currentState->OnEnter(*this);

    m_damagedSub = m_health->SubscribeDamaged(
        [this](int) {
            m_damaged = true;
        }
    );
}

void bomberman::PlayerControllerComponent::Update(float deltaTime) {
    if (m_damaged) {
        m_damaged = false;
        if (m_currentState->IsAlive()) {
            m_currentState->OnExit(*this);
            m_currentState = std::make_unique<DyingPlayerState>();
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

glm::ivec2 bomberman::PlayerControllerComponent::GetFacing() const {
    return m_movement->GetFacing();
}

bool bomberman::PlayerControllerComponent::IsMoving() const {
    return m_movement->IsMoving();
}

bool bomberman::PlayerControllerComponent::IsAnimationPlaying() const {
    return m_sprite->IsPlaying();
}

int bomberman::PlayerControllerComponent::GetLives() const {
    return m_health->GetLives();
}

void bomberman::PlayerControllerComponent::PlayAnimation(const SpriteDefinition &clip, bool loop) const {
    m_sprite->Play(clip, loop);
}

void bomberman::PlayerControllerComponent::Respawn() const {
    m_movement->Respawn(m_spawnCell);
}

void bomberman::PlayerControllerComponent::NotifyDied() const {
    bengine::ServiceLocator::GetEventBus().Broadcast(events::PlayerDied{
            .player = GetParent(),
        }
    );
}
