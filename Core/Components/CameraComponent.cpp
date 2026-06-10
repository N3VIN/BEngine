#include "CameraComponent.h"
#include "SceneGraph/GameObject.h"
#include "Renderer/Renderer.h"
#include <utility>

namespace bengine {
    CameraComponent::CameraComponent(GameObject *parent)
        : Component(parent) {}

    void CameraComponent::SetTarget(GameObject *target) {
        m_targets.assign(1, target);
    }

    void CameraComponent::SetTargets(std::vector<GameObject *> targets) {
        m_targets = std::move(targets);
    }

    void CameraComponent::SetTargetOffset(glm::vec2 offset) {
        m_targetOffset = offset;
    }

    void CameraComponent::SetLevelBounds(glm::vec2 origin, glm::vec2 size) {
        m_levelOrigin = origin;
        m_levelSize = size;
        m_hasBounds = true;
    }

    void CameraComponent::SetZoom(float zoom) {
        m_zoom = zoom;
    }

    float CameraComponent::GetZoom() const {
        return m_zoom;
    }

    glm::vec2 CameraComponent::GetViewCenter() const {
        glm::vec2 center = GetParent()->GetWorldPosition();

        if (!m_targets.empty()) {
            center = glm::vec2{0.0f, 0.0f};
            for (const auto *target: m_targets) {
                center += target->GetWorldPosition();
            }

            center /= static_cast<float>(m_targets.size());
        }

        center += m_targetOffset;

        if (!m_hasBounds) {
            return center;
        }

        const auto windowSize = Renderer::GetInstance().GetWindowSize();
        const auto viewHalf = windowSize / (2.0f * m_zoom);

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

    bool CameraComponent::IsWorldPointVisible(glm::vec2 worldPos, glm::vec2 margin) const {
        const glm::vec2 viewHalf = Renderer::GetInstance().GetWindowSize() / (2.0f * m_zoom);
        const glm::vec2 delta = glm::abs(worldPos - GetViewCenter());

        const bool scrollsX = !m_hasBounds || m_levelSize.x > viewHalf.x * 2.0f;
        const bool scrollsY = !m_hasBounds || m_levelSize.y > viewHalf.y * 2.0f;

        const bool visibleX = !scrollsX || delta.x <= viewHalf.x - margin.x;
        const bool visibleY = !scrollsY || delta.y <= viewHalf.y - margin.y;
        return visibleX && visibleY;
    }
}
