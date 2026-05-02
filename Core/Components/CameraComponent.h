#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae {
    class GameObject;

    class CameraComponent final : public Component {
    public:
        explicit CameraComponent(GameObject *parent);

        void SetTarget(GameObject *target);
        void SetLevelBounds(glm::vec2 origin, glm::vec2 size);
        void SetZoom(float zoom);

        [[nodiscard]] float GetZoom() const {
            return m_zoom;
        }

        [[nodiscard]] glm::vec2 GetViewCenter() const;

    private:
        float m_zoom{1.0f};
        GameObject *m_target{nullptr};
        bool m_hasBounds{false};
        glm::vec2 m_levelOrigin{};
        glm::vec2 m_levelSize{};
    };
}
