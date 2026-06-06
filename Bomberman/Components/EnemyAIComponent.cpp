#include "EnemyAIComponent.h"
#include "LevelGridComponent.h"
#include "GridMovementComponent.h"
#include "SceneGraph/GameObject.h"
#include "Random.h"

bomberman::EnemyAIComponent::EnemyAIComponent(bengine::GameObject *parent, LevelGridComponent *grid, EnemyType type, std::span<bengine::GameObject *const> players)
    : bengine::Component(parent)
  , m_grid(grid)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_players(players)
  , m_stats(&GetEnemyStats(type)) {
    Turn();
}

void bomberman::EnemyAIComponent::Update(float deltaTime) {
    Wander(deltaTime);
    LookForPlayer();
    m_movement->SetDesiredDirection(m_direction);
}

void bomberman::EnemyAIComponent::Wander(float deltaTime) {
    m_turnTimer.Update(deltaTime);

    const auto ahead = m_movement->GetCell() + m_direction;
    const bool blocked = !m_movement->IsMoving() && !m_grid->IsWalkable(ahead);
    if (m_turnTimer.IsExpired() || blocked) {
        Turn();
    }
}

void bomberman::EnemyAIComponent::Turn() {
    m_direction = DIRECTIONS[static_cast<size_t>(bengine::RandomInt(0, 3))];
    m_turnTimer.SetDuration(bengine::RandomFloat(m_stats->minInterval, m_stats->maxInterval));
}

void bomberman::EnemyAIComponent::LookForPlayer() {
    if (m_stats->sight == SightAxes::None) {
        return;
    }

    const auto cell = m_movement->IsMoving() ? m_movement->GetCell() + m_movement->GetFacing() : m_movement->GetCell();

    for (const auto *player: m_players) {
        const auto *movement = player->GetComponent<GridMovementComponent>();
        if (movement == nullptr) {
            continue;
        }

        const auto target = movement->GetCell();
        const auto direction = target - cell;
        const bool sameColumn = direction.x == 0 && SeesAlongColumn(m_stats->sight);
        const bool sameRow = direction.y == 0 && SeesAlongRow(m_stats->sight);

        if ((sameColumn || sameRow) && direction != glm::ivec2{0, 0} && m_grid->HasClearPath(cell, target)) {
            m_direction = glm::sign(direction);
            return;
        }
    }
}
