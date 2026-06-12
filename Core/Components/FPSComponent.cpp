#include "FPSComponent.h"
#include "SceneGraph/GameObject.h"
#include "TextComponent.h"

namespace bengine {
    FPSComponent::FPSComponent(GameObject *owner)
        : Component(owner) {
        m_text = owner->GetComponent<TextComponent>();
        m_text->SetText("0 FPS");
    }

    void FPSComponent::Update(float deltaTime) {
        ++m_frameCount;
        m_elapsedTime += deltaTime;

        if (m_elapsedTime >= updateInterval) {
            if (const int fps = static_cast<int>(m_frameCount / m_elapsedTime); fps != m_cachedFps) {
                m_cachedFps = fps;
                m_text->SetText(std::to_string(fps) + " FPS");
            }
            m_frameCount = 0;
            m_elapsedTime -= updateInterval;
        }
    }
}
