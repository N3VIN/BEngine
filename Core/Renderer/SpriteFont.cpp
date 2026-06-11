#include "Renderer/SpriteFont.h"

#include <cctype>
#include <SDL3/SDL.h>

#include "Renderer/ResourceManager.h"
#include "Renderer/Texture2D.h"

bengine::SpriteFont::SpriteFont(std::string_view atlasFile, int glyphWidth, int glyphHeight, const std::vector<GlyphRow> &rows)
    : m_atlas(ResourceManager::GetInstance().LoadTexture(atlasFile))
  , m_glyphWidth(glyphWidth)
  , m_glyphHeight(glyphHeight) {
    SDL_SetTextureScaleMode(m_atlas->GetSDLTexture(), SDL_SCALEMODE_NEAREST);

    for (const auto &row: rows) {
        for (size_t i = 0; i < row.chars.size(); ++i) {
            const SDL_Rect rect{row.originX + static_cast<int>(i) * glyphWidth, row.originY, glyphWidth, glyphHeight};
            m_glyphs.insert_or_assign(row.chars[i], rect);
        }
    }
}

const SDL_Rect *bengine::SpriteFont::TryGetGlyph(char character) const {
    auto it = m_glyphs.find(character);
    if (it == m_glyphs.end()) {
        const char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
        it = m_glyphs.find(upper);
    }

    return it == m_glyphs.end() ? nullptr : &it->second;
}
