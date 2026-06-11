#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_pixels.h>

#include "Component.h"

namespace bengine {
    class SpriteFont;

    class SpriteTextComponent final : public Component {
    public:
        explicit SpriteTextComponent(GameObject *owner);

        void Render() const override;

        void SetText(std::string_view text);
        void SetFont(std::shared_ptr<SpriteFont> font);
        void SetColor(const SDL_Color &color);
        void SetScale(float scale);
        void SetIgnoreCamera(bool ignore);
        void SetCentered(bool centered);

    private:
        std::string m_text{};
        SDL_Color m_color{255, 255, 255, 255};
        float m_scale{1.0f};
        bool m_ignoreCamera{true};
        bool m_centered{true};
        std::shared_ptr<SpriteFont> m_font{};
    };
} // bengine
