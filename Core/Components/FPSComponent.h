#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace bengine {
    constexpr float UPDATE_INTERVAL = 1.0f;

    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject *owner);

        void Update(float deltaTime) override;

    private:
        int m_frameCount{};
        int m_cachedFps{};
        float m_elapsedTime{UPDATE_INTERVAL};

        TextComponent *m_text{nullptr};
    };
} // bengine
