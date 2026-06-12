#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "Components/Component.h"
#include "Components/RenderComponent.h"
#include "Components/SpriteDefinition.h"
#include "Timer.h"

namespace bengine {
    class SpriteRendererComponent final : public Component {
    public:
        explicit SpriteRendererComponent(GameObject *parent);

        void Update(float deltaTime) override;
        void SetIgnoreCamera(bool ignore) const;

        void SetSpriteSheet(const std::string &texturePath, float scale, int tileSize);
        void SetSprite(const SpriteDefinition &definition, float fps);

        void PlayOnce(float fps);
        void Play(const SpriteDefinition &definition, bool loop, float fps);

        [[nodiscard]] bool IsPlaying() const;

    private:
        void ApplySourceRect(int frame) const;

        RenderComponent *m_renderComponent{};
        SpriteDefinition m_definition{};
        int m_tileSize{16};
        int m_currentFrame{0};
        bool m_playing{false};
        bool m_looping{true};
        Timer m_frameTimer{};
    };
}
