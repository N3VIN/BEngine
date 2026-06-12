#include "BombComponent.h"

bomberman::BombComponent::BombComponent(bengine::GameObject *parent, glm::ivec2 cell, int blastRadius, bengine::GameObject *owner, float fuseDuration, bool remote)
    : bengine::Component(parent)
  , m_cell(cell)
  , m_blastRadius(blastRadius)
  , m_owner(owner)
  , m_fuse(fuseDuration)
  , m_remote(remote) {}

void bomberman::BombComponent::MarkDetonated() {
    m_detonated = true;
}

void bomberman::BombComponent::TickFuse(float deltaTime) {
    if (!m_remote) {
        m_fuse.Update(deltaTime);
    }
}

bool bomberman::BombComponent::IsDetonated() const {
    return m_detonated;
}

bool bomberman::BombComponent::FuseExpired() const {
    return !m_remote && m_fuse.IsExpired();
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
