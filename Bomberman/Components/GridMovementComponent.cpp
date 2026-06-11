#include "GridMovementComponent.h"
#include "LevelGridComponent.h"
#include "Components/CameraComponent.h"
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

void bomberman::GridMovementComponent::SetViewClamp(const bengine::CameraComponent *camera) {
    m_viewClamp = camera;
}

void bomberman::GridMovementComponent::SetMovementEnabled(bool enabled) {
    m_movementEnabled = enabled;
}

void bomberman::GridMovementComponent::Respawn(glm::ivec2 cell) {
    m_cell = cell;
    m_activeDir = {0, 0};
    m_queuedDir = {0, 0};
    m_progress = 0.f;
    m_facing = {0, 1};
    m_movementEnabled = true;
    ApplyVisualPosition();
}

void bomberman::GridMovementComponent::Update(float deltaTime) {
    if (!m_movementEnabled) {
        m_activeDir = {0, 0};
        m_queuedDir = {0, 0};
        m_progress = 0.f;
        ApplyVisualPosition();
        return;
    }

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

    const glm::ivec2 targetCell = m_cell + m_queuedDir;
    if (!m_levelGridComponent->IsWalkable(targetCell)) {
        return false;
    }

    if (m_viewClamp != nullptr) {
        const float cellSize = m_levelGridComponent->GetCellSize();
        const auto cellCenter = m_levelGridComponent->CellToWorld(targetCell) + glm::vec2{cellSize * 0.5f};
        if (!m_viewClamp->IsWorldPointVisible(cellCenter, glm::vec2{cellSize, cellSize})) {
            return false;
        }
    }

    m_activeDir = m_queuedDir;
    m_facing = m_activeDir;
    return true;
}

void bomberman::GridMovementComponent::ApplyVisualPosition() const {
    const auto base = m_levelGridComponent->CellToWorld(m_cell);
    const auto offset = glm::vec2(m_activeDir) * m_levelGridComponent->GetCellSize() * m_progress;
    GetParent()->SetLocalPosition(base + offset);
}
