#include "BombComponent.h"

bomberman::BombComponent::BombComponent(bengine::GameObject *parent, glm::ivec2 cell, float fuseTime, int blastRadius)
    : bengine::Component(parent)
  , m_cell(cell)
  , m_fuseTimer(fuseTime)
  , m_blastRadius(blastRadius) {}

bool bomberman::BombComponent::AdvanceFuse(float deltaTime) {
    if (m_detonated) {
        return false;
    }

    m_fuseTimer.Update(deltaTime);
    return m_fuseTimer.IsExpired();
}

void bomberman::BombComponent::MarkDetonated() {
    m_detonated = true;
}

bool bomberman::BombComponent::IsDetonated() const {
    return m_detonated;
}

glm::ivec2 bomberman::BombComponent::GetCell() const {
    return m_cell;
}

int bomberman::BombComponent::GetRadius() const {
    return m_blastRadius;
}

bengine::GameObject *bomberman::BombComponent::GetGameObject() const {
    return GetParent();
}
