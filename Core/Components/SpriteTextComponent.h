#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_pixels.h>

#include "Component.h"
#include "Colors.h"

namespace bengine {
    class SpriteFont;

    enum class TextAlign {
        Left,
        Center,
        Right
    };

    class SpriteTextComponent final : public Component {
    public:
        explicit SpriteTextComponent(GameObject *owner);

        void Render() const override;

        void SetText(std::string_view text);
        void SetFont(std::shared_ptr<SpriteFont> font);
        void SetColor(const SDL_Color &color);
        void SetScale(float scale);
        void SetIgnoreCamera(bool ignore);
        void SetAlignment(TextAlign align);

    private:
        std::string m_text{};
        SDL_Color m_color{colors::white};
        float m_scale{1.0f};
        bool m_ignoreCamera{true};
        TextAlign m_align{TextAlign::Center};
        std::shared_ptr<SpriteFont> m_font{};
    };
} // bengine
