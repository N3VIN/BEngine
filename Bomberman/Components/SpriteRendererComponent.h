#pragma once
#include <SDL3/SDL.h>
#include "Components/Component.h"
#include "Level/Tileset.h"
#include "Timer.h"

namespace dae {
    class RenderComponent;

    class SpriteRendererComponent final : public Component {
    public:
        SpriteRendererComponent(GameObject *parent, SpriteType spriteType);

        void Update(float deltaTime) override;
        void SetIgnoreCamera(bool ignore) const;

        void PlayOnce(float fps);

        [[nodiscard]] bool IsPlaying() const {
            return m_playing;
        }

        static constexpr float ANIMATION_FPS = 8.f;

    private:
        void ApplySourceRect(int frame);

        RenderComponent *m_renderComponent{};
        int m_col{};
        int m_row{};
        int m_numCols{1};
        int m_numRows{1};
        int m_tileSize{16};
        int m_frameCount{1};
        int m_currentFrame{0};
        int m_frameColumns{1};
        bool m_playing{false};
        bool m_looping{true};
        Timer m_frameTimer{};
    };
}
