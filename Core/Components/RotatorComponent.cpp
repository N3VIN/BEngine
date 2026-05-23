#include "RotatorComponent.h"
#include <numbers>
#include "SceneGraph/GameObject.h"

bengine::RotatorComponent::RotatorComponent(GameObject *parent)
    : Component(parent) {}

void bengine::RotatorComponent::SetSpeed(float period, bool isClockwise) {
    auto direction = isClockwise ? 1.f : -1.f;

    m_speed = direction * 2 * std::numbers::pi_v<float> / period;
}

void bengine::RotatorComponent::SetRadius(float radius) {
    m_radius = radius;
}

void bengine::RotatorComponent::Update(float deltaTime) {
    m_angle += m_speed * deltaTime;

    m_angle = std::fmod(m_angle, std::numbers::pi_v<float> * 2.0f);
    if (m_angle < 0.0f) {
        m_angle += std::numbers::pi_v<float> * 2.0f;
    }
    const float x = m_radius * std::cos(m_angle);
    const float y = m_radius * std::sin(m_angle);

    GetParent()->SetLocalPosition(glm::vec2(x, y));
}
