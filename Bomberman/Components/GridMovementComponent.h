#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>

namespace bomberman {
    class LevelGridComponent;

    class GridMovementComponent final : public bengine::Component {
    public:
        GridMovementComponent(bengine::GameObject *parent, LevelGridComponent *levelGridComponent, glm::ivec2 startCell, float cellsPerSecond);
        void Update(float deltaTime) override;
        void SetDesiredDirection(glm::ivec2 direction);
        void Respawn(glm::ivec2 cell);

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

        [[nodiscard]] glm::ivec2 GetFacing() const {
            return m_facing;
        }

        [[nodiscard]] bool IsMoving() const {
            return m_activeDir != glm::ivec2{0, 0};
        }

    private:
        void ApplyVisualPosition() const;
        bool TryStartMoveInQueuedDir();

        LevelGridComponent *m_levelGridComponent;
        glm::ivec2 m_cell{0, 0};
        glm::ivec2 m_activeDir{0, 0};
        glm::ivec2 m_queuedDir{0, 0};
        glm::ivec2 m_facing{0, 1};
        float m_progress{0.f};
        const float m_cellsPerSecond;
    };
}
