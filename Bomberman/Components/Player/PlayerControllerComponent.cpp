#include "PlayerControllerComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "Gameplay/GameEvents.h"
#include "States/PlayerStates.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/GameObject.h"
#include "utils.h"

bomberman::PlayerControllerComponent::PlayerControllerComponent(bengine::GameObject *parent, PlayerSprites sprites)
    : bengine::Component(parent)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<bengine::SpriteRendererComponent>())
  , m_health(parent->GetComponent<HealthComponent>())
  , m_sprites(sprites)
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
            bengine::ServiceLocator::GetAudioService().PlayAudio(utils::Hash("player_death"), 1.0f);
            m_currentState->OnExit(*this);
            m_currentState = std::make_unique<DyingPlayerState>();
            m_currentState->OnEnter(*this);
            return;
        }
    }

    if (auto next = m_currentState->Update(*this, deltaTime)) {
        m_currentState->OnExit(*this);
        m_currentState = std::move(next); // update the state
        m_currentState->OnEnter(*this);
    }

    const auto cell = m_movement->GetCell();
    if (m_currentState->IsAlive() && IsMoving() && cell != m_lastStepCell) {
        PlayStep(GetFacing()); // play sound
    }

    m_lastStepCell = cell;
}

const bomberman::PlayerSprites &bomberman::PlayerControllerComponent::GetSprites() const {
    return m_sprites;
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
    m_sprite->Play(clip, loop, GetTileset().animationFps);
}

void bomberman::PlayerControllerComponent::StopMovement() const {
    m_movement->SetMovementEnabled(false);
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

void bomberman::PlayerControllerComponent::PlayStep(glm::ivec2 facing) const {
    const auto step = facing.x != 0 ? utils::Hash("step_horizontal") : utils::Hash("step_vertical"); // chose direction
    bengine::ServiceLocator::GetAudioService().PlayAudio(step, 1.0f);
}
