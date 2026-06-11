#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include <SDL3/SDL_rect.h>

namespace bengine {
    class Texture2D;

    class SpriteFont final {
    public:
        struct GlyphRow {
            std::string chars{};
            int originX{};
            int originY{};
        };

        SpriteFont(std::string_view atlasFile, int glyphWidth, int glyphHeight, const std::vector<GlyphRow> &rows);

        [[nodiscard]] const SDL_Rect *TryGetGlyph(char character) const;
        [[nodiscard]] Texture2D &GetAtlas() const;
        [[nodiscard]] glm::ivec2 GlyphSize() const;

    private:
        std::shared_ptr<Texture2D> m_atlas{};
        int m_glyphWidth{};
        int m_glyphHeight{};
        std::unordered_map<char, SDL_Rect> m_glyphs{};
    };

    inline Texture2D &SpriteFont::GetAtlas() const {
        return *m_atlas;
    }

    inline glm::ivec2 SpriteFont::GlyphSize() const {
        return {m_glyphWidth, m_glyphHeight};
    }
}
