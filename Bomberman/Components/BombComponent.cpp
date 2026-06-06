#include "BombComponent.h"

bomberman::BombComponent::BombComponent(bengine::GameObject *parent, glm::ivec2 cell, int blastRadius, bengine::GameObject *owner)
    : bengine::Component(parent)
  , m_cell(cell)
  , m_blastRadius(blastRadius)
  , m_owner(owner) {}

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

bengine::GameObject *bomberman::BombComponent::GetOwner() const {
    return m_owner;
}

bengine::GameObject *bomberman::BombComponent::GetGameObject() const {
    return GetParent();
}
