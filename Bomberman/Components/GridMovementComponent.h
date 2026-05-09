#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>

namespace dae {
    class LevelGridComponent;

    class GridMovementComponent final : public Component {
    public:
        GridMovementComponent(GameObject *parent, LevelGridComponent *levelGridComponent, glm::ivec2 startCell, float cellsPerSecond);
        void Update(float deltaTime) override;
        void SetDesiredDirection(glm::ivec2 direction);

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

    private:
        void ApplyVisualPosition() const;
        bool TryStartMoveInQueuedDir();

        [[nodiscard]] bool IsMoving() const {
            return m_activeDir != glm::ivec2{0, 0};
        }

        LevelGridComponent *m_levelGridComponent;
        glm::ivec2 m_cell{0, 0};
        glm::ivec2 m_activeDir{0, 0};
        glm::ivec2 m_queuedDir{0, 0};
        float m_progress{0.f};
        float m_cellsPerSecond;
    };
}
