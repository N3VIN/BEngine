#include "SceneGraph/Transform.h"

void dae::Transform::SetLocalPosition(const glm::vec3 &position) {
    m_localPosition = position;
}

void dae::Transform::SetLocalRotation(const float radians) {
    m_localRotation = radians;
}

void dae::Transform::SetLocalScale(const glm::vec2 &scale) {
    m_localScale = scale;
}

void dae::Transform::SetLocalFromMatrix(const glm::mat3 &matrix) {
    m_localPosition = glm::vec3(matrix[2][0], matrix[2][1], 0.f);
    m_localRotation = std::atan2(matrix[0][1], matrix[0][0]);
    m_localScale = {
        glm::length(glm::vec2(matrix[0][0], matrix[0][1])),
        glm::length(glm::vec2(matrix[1][0], matrix[1][1]))
    };
}

const glm::vec3 &dae::Transform::GetLocalPosition() const {
    return m_localPosition;
}

float dae::Transform::GetLocalRotation() const {
    return m_localRotation;
}

const glm::vec2 &dae::Transform::GetLocalScale() const {
    return m_localScale;
}

void dae::Transform::SetDirty() {
    m_isDirty = true;
}

bool dae::Transform::IsDirty() const {
    return m_isDirty;
}

void dae::Transform::UpdateWorldTransform(const glm::mat3 &parentWorld) {
    m_worldTransform = parentWorld * ComputeLocalTransform();
    m_isDirty = false;
}

void dae::Transform::UpdateWorldTransform() {
    m_worldTransform = ComputeLocalTransform();
    m_isDirty = false;
}

const glm::mat3 &dae::Transform::GetWorldTransform() const {
    return m_worldTransform;
}

glm::vec2 dae::Transform::GetWorldPosition() const {
    return {m_worldTransform[2][0], m_worldTransform[2][1]};
}

float dae::Transform::GetWorldRotation() const {
    return std::atan2(m_worldTransform[0][1], m_worldTransform[0][0]);
}

glm::vec2 dae::Transform::GetWorldScale() const {
    const float sx = glm::length(glm::vec2(m_worldTransform[0][0], m_worldTransform[0][1]));
    const float sy = glm::length(glm::vec2(m_worldTransform[1][0], m_worldTransform[1][1]));
    return {sx, sy};
}

glm::mat3 dae::Transform::ComputeLocalTransform() const {
    const float cosR = std::cos(m_localRotation);
    const float sinR = std::sin(m_localRotation);

    // 2D TRS column major matrix
    // [cos*sx, sin*sx, 0]
    // [-sin*sy, cos*sy, 0]
    // [tx, ty, 1]
    glm::mat3 transform{1.f};
    transform[0][0] = cosR * m_localScale.x;
    transform[0][1] = sinR * m_localScale.x;
    transform[1][0] = -sinR * m_localScale.y;
    transform[1][1] = cosR * m_localScale.y;
    transform[2][0] = m_localPosition.x;
    transform[2][1] = m_localPosition.y;
    return transform;
}
