#include "EnemyStates.h"
#include "Components/EnemyControllerComponent.h"

bomberman::EnemyWalkState::EnemyWalkState(bool facingLeft)
    : m_facingLeft(facingLeft) {}

void bomberman::EnemyWalkState::OnEnter(EnemyControllerComponent &controller) {
    controller.PlayWalkAnimation(m_facingLeft);
}

std::unique_ptr<bomberman::IEnemyState> bomberman::EnemyWalkState::Update(EnemyControllerComponent &controller, float /*deltaTime*/) {
    const auto facing = controller.GetFacing();
    if (facing.x == 0) {
        return nullptr;
    }

    if (facing.x < 0 == m_facingLeft) {
        return nullptr;
    }

    return MakeEnemyWalkState(facing);
}

void bomberman::EnemyDyingState::OnEnter(EnemyControllerComponent &controller) {
    controller.StopMovement();
    controller.PlayDeathAnimation();
}

std::unique_ptr<bomberman::IEnemyState> bomberman::EnemyDyingState::Update(EnemyControllerComponent &controller, float /*deltaTime*/) {
    if (controller.IsAnimationPlaying()) {
        return nullptr;
    }

    controller.Die();
    return nullptr;
}

bool bomberman::EnemyDyingState::IsAlive() const {
    return false;
}

std::unique_ptr<bomberman::IEnemyState> bomberman::MakeEnemyWalkState(glm::ivec2 facing) {
    return std::make_unique<EnemyWalkState>(facing.x < 0);
}
