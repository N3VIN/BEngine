#pragma once
#include "Components/Component.h"
#include <glm/glm.hpp>

namespace bengine {
    class CameraComponent;
}

namespace bomberman {
    class LevelGridComponent;

    class GridMovementComponent final : public bengine::Component {
    public:
        GridMovementComponent(bengine::GameObject *parent, LevelGridComponent *levelGridComponent, glm::ivec2 startCell, float cellsPerSecond);
        void Update(float deltaTime) override;
        void SetDesiredDirection(glm::ivec2 direction);
        void SetViewClamp(const bengine::CameraComponent *camera);
        void SetMovementEnabled(bool enabled);
        void Respawn(glm::ivec2 cell);

        [[nodiscard]] glm::ivec2 GetCell() const;
        [[nodiscard]] glm::ivec2 GetFacing() const;
        [[nodiscard]] bool IsMoving() const;

    private:
        void ApplyVisualPosition() const;
        bool TryStartMoveInQueuedDir();

        LevelGridComponent *m_levelGridComponent;
        const bengine::CameraComponent *m_viewClamp{nullptr};
        glm::ivec2 m_cell{0, 0};
        glm::ivec2 m_activeDir{0, 0};
        glm::ivec2 m_queuedDir{0, 0};
        glm::ivec2 m_facing{0, 1};
        float m_progress{0.f};
        const float m_cellsPerSecond;
        bool m_movementEnabled{true};
    };
}
