#include "PlayerStateComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "HealthComponent.h"
#include "GameEvents.h"
#include "States/PlayerStates.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"

bomberman::PlayerStateComponent::PlayerStateComponent(bengine::GameObject *parent)
    : bengine::Component(parent)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<SpriteRendererComponent>())
  , m_health(parent->GetComponent<HealthComponent>())
  , m_spawnCell(m_movement->GetCell()) {
    m_currentState = std::make_unique<IdlePlayerState>();
    m_currentState->OnEnter(*this);

    m_damagedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::PlayerDamaged>(
        [this](const events::PlayerDamaged &event) {
            if (event.player == GetParent()) {
                m_damaged = true;
            }
        }
    );
}

void bomberman::PlayerStateComponent::Update(float deltaTime) {
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
