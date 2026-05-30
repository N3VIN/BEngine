#pragma once
#include <glm/vec2.hpp>
#include <string>

struct SDL_Texture;

namespace bengine {
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Texture2D final {
    public:
        [[nodiscard]] SDL_Texture *GetSDLTexture() const;
        explicit Texture2D(SDL_Texture *texture);
        explicit Texture2D(std::string_view fullPath);
        ~Texture2D();

        [[nodiscard]] glm::vec2 GetSize() const;

        Texture2D(const Texture2D &) = delete;
        Texture2D(Texture2D &&) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &&) = delete;

    private:
        SDL_Texture *m_texture{nullptr};
    };
}
