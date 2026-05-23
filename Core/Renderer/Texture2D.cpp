#include "Texture2D.h"
#include "Renderer.h"

bengine::Texture2D::~Texture2D() {
    SDL_DestroyTexture(m_texture);
}

glm::vec2 bengine::Texture2D::GetSize() const {
    float w{}, h{};
    SDL_GetTextureSize(m_texture, &w, &h);
    return {w, h};
}

SDL_Texture *bengine::Texture2D::GetSDLTexture() const {
    return m_texture;
}

bengine::Texture2D::Texture2D(std::string_view fullPath) {
    SDL_Surface *surface = SDL_LoadPNG(fullPath.data());
    if (!surface) {
        throw std::runtime_error(
            std::string("Failed to load PNG: ") + SDL_GetError()
        );
    }

    m_texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        surface
    );

    SDL_DestroySurface(surface);

    if (!m_texture) {
        throw std::runtime_error(
            std::string("Failed to create texture from surface: ") + SDL_GetError()
        );
    }
}

bengine::Texture2D::Texture2D(SDL_Texture *texture)
    : m_texture{texture} {
    assert(m_texture != nullptr);
}
