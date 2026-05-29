#include "PlayerStates.h"
#include "Components/PlayerStateComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

static const bomberman::SpriteDefinition &WalkClip(glm::ivec2 facing) {
    const auto &player = bomberman::GetTileset().player;
    if (facing.x < 0) {
        return player.walkLeft;
    }
    if (facing.x > 0) {
        return player.walkRight;
    }
    if (facing.y < 0) {
        return player.walkUp;
    }

    return player.walkDown;
}

void bomberman::IdlePlayerState::OnEnter(PlayerStateComponent &state) {
    SpriteDefinition standing = WalkClip(state.GetMovement()->GetFacing());
    standing.frameCount = 1;
    state.GetSprite()->Play(standing, false);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::IdlePlayerState::Update(PlayerStateComponent &state, float /*deltaTime*/) {
    if (const auto *movement = state.GetMovement(); movement->IsMoving()) {
        return MakeWalkState(movement->GetFacing());
    }

    return nullptr;
}

bomberman::DirectionalWalkState::DirectionalWalkState(glm::ivec2 direction, const SpriteDefinition &clip)
    : m_direction(direction)
  , m_clip(clip) {}

void bomberman::DirectionalWalkState::OnEnter(PlayerStateComponent &state) {
    state.GetSprite()->Play(m_clip, true);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::DirectionalWalkState::Update(PlayerStateComponent &state, float /*deltaTime*/) {
    const auto *movement = state.GetMovement();
    if (!movement->IsMoving()) {
        return std::make_unique<IdlePlayerState>();
    }

    if (movement->GetFacing() != m_direction) {
        return MakeWalkState(movement->GetFacing());
    }

    return nullptr;
}

bomberman::WalkUpState::WalkUpState()
    : DirectionalWalkState({0, -1}, GetTileset().player.walkUp) {}

bomberman::WalkDownState::WalkDownState()
    : DirectionalWalkState({0, 1}, GetTileset().player.walkDown) {}

bomberman::WalkLeftState::WalkLeftState()
    : DirectionalWalkState({-1, 0}, GetTileset().player.walkLeft) {}

bomberman::WalkRightState::WalkRightState()
    : DirectionalWalkState({1, 0}, GetTileset().player.walkRight) {}

void bomberman::DyingPlayerState::OnEnter(PlayerStateComponent &state) {
    state.GetSprite()->Play(GetTileset().player.death, false);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::DyingPlayerState::Update(PlayerStateComponent &state, float /*deltaTime*/) {
    if (state.GetSprite()->IsPlaying()) {
        return nullptr;
    }

    if (state.GetHealth()->GetLives() > 0) {
        state.GetMovement()->Respawn(state.GetSpawnCell());
        return std::make_unique<IdlePlayerState>();
    }

    bengine::ServiceLocator::GetEventBus().Broadcast(events::PlayerDied{
            .player = state.GetPlayerObject(),
        }
    );

    return std::make_unique<DeadPlayerState>();
}

std::unique_ptr<bomberman::IPlayerState> bomberman::MakeWalkState(glm::ivec2 facing) {
    if (facing.x < 0) {
        return std::make_unique<WalkLeftState>();
    }
    if (facing.x > 0) {
        return std::make_unique<WalkRightState>();
    }
    if (facing.y < 0) {
        return std::make_unique<WalkUpState>();
    }

    return std::make_unique<WalkDownState>();
}
