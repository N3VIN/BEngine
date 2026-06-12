#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_pixels.h>

#include "Component.h"
#include "Colors.h"

namespace bengine {
    class Font;
    class Texture2D;

    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject *owner);

        void Update(float deltaTime) override;

        void SetText(std::string_view text);
        void SetFont(std::shared_ptr<Font> font);
        void SetColor(const SDL_Color &color);

    private:
        bool m_needsUpdate{true};
        std::string m_text{};
        SDL_Color m_color{colors::white};
        std::shared_ptr<Font> m_font{};
        std::shared_ptr<Texture2D> m_textTexture{};
    };
} // bengine
