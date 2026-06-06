#include "EnemyStates.h"
#include "Components/EnemyStateComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"

bomberman::EnemyWalkState::EnemyWalkState(bool facingLeft, const SpriteDefinition &clip)
    : m_facingLeft(facingLeft)
  , m_clip(clip) {}

void bomberman::EnemyWalkState::OnEnter(EnemyStateComponent &state) {
    state.GetSprite()->Play(m_clip, true);
}

std::unique_ptr<bomberman::IEnemyState> bomberman::EnemyWalkState::Update(EnemyStateComponent &state, float /*deltaTime*/) {
    const auto facing = state.GetMovement()->GetFacing();
    if (facing.x == 0) {
        return nullptr;
    }
    if (facing.x < 0 == m_facingLeft) {
        return nullptr;
    }

    return MakeEnemyWalkState(facing, state.GetSprites());
}

void bomberman::EnemyDyingState::OnEnter(EnemyStateComponent &state) {
    state.GetSprite()->Play(state.GetSprites().death, false);
}

std::unique_ptr<bomberman::IEnemyState> bomberman::EnemyDyingState::Update(EnemyStateComponent &state, float /*deltaTime*/) {
    if (state.GetSprite()->IsPlaying()) {
        return nullptr;
    }

    state.Die();
    return nullptr;
}

std::unique_ptr<bomberman::IEnemyState> bomberman::MakeEnemyWalkState(glm::ivec2 facing, const EnemySprites &sprites) {
    const bool facingLeft = facing.x < 0;
    return std::make_unique<EnemyWalkState>(facingLeft, facingLeft ? sprites.walkLeft : sprites.walkRight);
}
