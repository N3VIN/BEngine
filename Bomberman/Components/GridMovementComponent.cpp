#include "GridMovementComponent.h"
#include "LevelGridComponent.h"
#include "SceneGraph/GameObject.h"

bomberman::GridMovementComponent::GridMovementComponent(bengine::GameObject *parent, LevelGridComponent *levelGridComponent, glm::ivec2 startCell, float cellsPerSecond)
    : bengine::Component(parent)
  , m_levelGridComponent(levelGridComponent)
  , m_cell(startCell)
  , m_cellsPerSecond(cellsPerSecond) {
    ApplyVisualPosition();
}

void bomberman::GridMovementComponent::SetDesiredDirection(glm::ivec2 direction) {
    m_queuedDir = direction;
}

void bomberman::GridMovementComponent::Update(float deltaTime) {
    if (!IsMoving()) {
        TryStartMoveInQueuedDir();
    }

    if (IsMoving()) {
        m_progress += m_cellsPerSecond * deltaTime;
        while (m_progress >= 1.f) {
            m_cell += m_activeDir;
            m_progress -= 1.f;

            if (!TryStartMoveInQueuedDir()) {
                m_activeDir = {0, 0};
                m_progress = 0.f;
                break;
            }
        }
    }

    ApplyVisualPosition();
    m_queuedDir = {0, 0};
}

bool bomberman::GridMovementComponent::TryStartMoveInQueuedDir() {
    if (m_queuedDir == glm::ivec2{0, 0}) {
        return false;
    }

    if (!m_levelGridComponent->IsWalkable(m_cell + m_queuedDir)) {
        return false;
    }

    m_activeDir = m_queuedDir;
    return true;
}

void bomberman::GridMovementComponent::ApplyVisualPosition() const {
    const glm::vec2 base = m_levelGridComponent->CellToWorld(m_cell);
    const glm::vec2 offset = glm::vec2(m_activeDir) * m_levelGridComponent->GetCellSize() * m_progress;
    GetParent()->SetLocalPosition(base + offset);
}
