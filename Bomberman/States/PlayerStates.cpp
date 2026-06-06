#include "PlayerStates.h"
#include "Components/PlayerControllerComponent.h"

void bomberman::IdlePlayerState::OnEnter(PlayerControllerComponent &controller) {
    auto standing = GetWalkClip(controller.GetFacing());
    standing.frameCount = 1;
    controller.PlayAnimation(standing, false);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::IdlePlayerState::Update(PlayerControllerComponent &controller, float /*deltaTime*/) {
    if (controller.IsMoving()) {
        return MakeWalkState(controller.GetFacing());
    }

    return nullptr;
}

bomberman::DirectionalWalkState::DirectionalWalkState(glm::ivec2 direction, const SpriteDefinition &clip)
    : m_direction(direction)
  , m_clip(clip) {}

void bomberman::DirectionalWalkState::OnEnter(PlayerControllerComponent &controller) {
    controller.PlayAnimation(m_clip, true);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::DirectionalWalkState::Update(PlayerControllerComponent &controller, float /*deltaTime*/) {
    if (!controller.IsMoving()) {
        return std::make_unique<IdlePlayerState>();
    }

    if (controller.GetFacing() != m_direction) {
        return MakeWalkState(controller.GetFacing());
    }

    return nullptr;
}

void bomberman::DyingPlayerState::OnEnter(PlayerControllerComponent &controller) {
    controller.PlayAnimation(GetTileset().player.death, false);
}

std::unique_ptr<bomberman::IPlayerState> bomberman::DyingPlayerState::Update(PlayerControllerComponent &controller, float /*deltaTime*/) {
    if (controller.IsAnimationPlaying()) {
        return nullptr;
    }

    if (controller.GetLives() > 0) {
        controller.Respawn();
        return std::make_unique<IdlePlayerState>();
    }

    controller.NotifyDied();
    return std::make_unique<DeadPlayerState>();
}

bool bomberman::DyingPlayerState::IsAlive() const {
    return false;
}

bool bomberman::DeadPlayerState::IsAlive() const {
    return false;
}

bomberman::SpriteDefinition bomberman::GetWalkClip(glm::ivec2 facing) {
    const auto &player = GetTileset().player;
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

std::unique_ptr<bomberman::IPlayerState> bomberman::MakeWalkState(glm::ivec2 facing) {
    return std::make_unique<DirectionalWalkState>(facing, GetWalkClip(facing));
}
