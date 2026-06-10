#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

namespace bengine {
    class GameObject;

    class CameraComponent final : public Component {
    public:
        explicit CameraComponent(GameObject *parent);

        void SetTarget(GameObject *target);
        void SetTargets(std::vector<GameObject *> targets);
        void SetTargetOffset(glm::vec2 offset);
        void SetLevelBounds(glm::vec2 origin, glm::vec2 size);
        void SetZoom(float zoom);

        [[nodiscard]] float GetZoom() const;
        [[nodiscard]] glm::vec2 GetViewCenter() const;
        [[nodiscard]] bool IsWorldPointVisible(glm::vec2 worldPos, glm::vec2 margin) const;

    private:
        float m_zoom{1.0f};
        std::vector<GameObject *> m_targets{};
        glm::vec2 m_targetOffset{};
        bool m_hasBounds{false};
        glm::vec2 m_levelOrigin{};
        glm::vec2 m_levelSize{};
    };
}
