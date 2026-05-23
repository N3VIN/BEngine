#include "Font.h"

TTF_Font *bengine::Font::GetFont() const {
    return m_font;
}

bengine::Font::Font(std::string_view fullPath, float size)
    : m_font(nullptr) {
    m_font = TTF_OpenFont(fullPath.data(), size);

    if (m_font == nullptr) {
        throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
    }
}

bengine::Font::~Font() {
    TTF_CloseFont(m_font);
}
