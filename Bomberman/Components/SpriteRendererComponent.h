#pragma once
#include <SDL3/SDL.h>
#include "Components/Component.h"
#include "Components/RenderComponent.h"
#include "Level/Tileset.h"
#include "Timer.h"

namespace bomberman {
    class SpriteRendererComponent final : public bengine::Component {
    public:
        SpriteRendererComponent(bengine::GameObject *parent, SpriteType spriteType);
        explicit SpriteRendererComponent(bengine::GameObject *parent);

        void Update(float deltaTime) override;
        void SetIgnoreCamera(bool ignore) const;

        void PlayOnce(float fps);
        void Play(const SpriteDefinition &definition, bool loop, float fps = ANIMATION_FPS);

        [[nodiscard]] bool IsPlaying() const {
            return m_playing;
        }

        static constexpr float ANIMATION_FPS = 8.f;

    private:
        void ApplySourceRect(int frame) const;

        bengine::RenderComponent *m_renderComponent{};
        int m_col{};
        int m_row{};
        int m_numCols{1};
        int m_numRows{1};
        int m_tileSize{16};
        int m_frameCount{1};
        int m_currentFrame{0};
        int m_frameColumns{1};
        int m_frameStrideCols{0};
        int m_frameStrideRows{0};
        bool m_playing{false};
        bool m_looping{true};
        bengine::Timer m_frameTimer{};
    };
}
