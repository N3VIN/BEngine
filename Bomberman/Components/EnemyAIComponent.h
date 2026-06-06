#pragma once
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "EnemyType.h"
#include "Timer.h"

namespace bomberman {
    class LevelGridComponent;
    class GridMovementComponent;
    class EnemyStateComponent;

    constexpr std::array<glm::ivec2, 4> DIRECTIONS{
        {
            {0, -1}, // down
            {0, 1},  // up
            {-1, 0}, // left
            {1, 0}   // right
        }
    };

    class EnemyAIComponent final : public bengine::Component {
    public:
        EnemyAIComponent(bengine::GameObject *parent, LevelGridComponent *grid, EnemyType type, const std::vector<bengine::GameObject *> *players);
        void Update(float deltaTime) override;

    private:
        void Wander(float deltaTime);
        void LookForPlayer();
        void Turn();

        LevelGridComponent *m_grid;
        GridMovementComponent *m_movement;
        EnemyStateComponent *m_state;
        const std::vector<bengine::GameObject *> *m_players;
        const EnemyStats *m_stats;
        glm::ivec2 m_direction{0, 0};
        bengine::Timer m_turnTimer{};
    };
}
