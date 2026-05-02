#include "CameraComponent.h"
#include "../SceneGraph/GameObject.h"
#include "../Renderer/Renderer.h"

namespace dae {
    CameraComponent::CameraComponent(GameObject *parent)
        : Component(parent) {}

    void CameraComponent::SetTarget(GameObject *target) {
        m_target = target;
    }

    void CameraComponent::SetLevelBounds(glm::vec2 origin, glm::vec2 size) {
        m_levelOrigin = origin;
        m_levelSize = size;
        m_hasBounds = true;
    }

    void CameraComponent::SetZoom(float zoom) {
        m_zoom = zoom;
    }

    glm::vec2 CameraComponent::GetViewCenter() const {
        glm::vec2 center = m_target ? m_target->GetWorldPosition() : glm::vec2{GetParent()->GetWorldPosition()};

        if (!m_hasBounds) {
            return center;
        }

        const auto windowSize = Renderer::GetInstance().GetWindowSize();
        const glm::vec2 viewHalf = windowSize / (2.0f * m_zoom);

        if (m_levelSize.x > viewHalf.x * 2.0f) {
            center.x = glm::clamp(center.x, m_levelOrigin.x + viewHalf.x, m_levelOrigin.x + m_levelSize.x - viewHalf.x); // clamp camera x axis
        }
        else {
            center.x = m_levelOrigin.x + m_levelSize.x * 0.5f;
        }

        if (m_levelSize.y > viewHalf.y * 2.0f) {
            center.y = glm::clamp(center.y, m_levelOrigin.y + viewHalf.y, m_levelOrigin.y + m_levelSize.y - viewHalf.y); // clamp camera y axis
        }
        else {
            center.y = m_levelOrigin.y + m_levelSize.y * 0.5f;
        }

        return center;
    }
}
