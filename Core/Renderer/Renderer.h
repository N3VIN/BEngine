#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "Patterns/Singleton.h"

namespace bengine {
    class Texture2D;
    class CameraComponent;

    class Renderer final : public Singleton<Renderer> {
    public:
        void Init(SDL_Window *window);
        void Render() const;
        void Destroy();

        void RenderTexture(const Texture2D &texture, const SDL_FRect &dst, const SDL_Rect *src, float angleDegrees) const;

        [[nodiscard]] SDL_Renderer *GetSDLRenderer() const;
        [[nodiscard]] const CameraComponent *GetActiveCamera() const;
        [[nodiscard]] glm::vec2 GetWindowSize() const;
        [[nodiscard]] glm::vec2 WorldToScreen(const glm::vec2 &worldPos) const;
        [[nodiscard]] const SDL_Color &GetBackgroundColor() const;

        void SetBackgroundColor(const SDL_Color &color);
        void SetViewOffset(const glm::vec2 &offset);
        void SetActiveCamera(const CameraComponent *camera);

    private:
        SDL_Renderer *m_renderer{};
        SDL_Window *m_window{};
        SDL_Color m_clearColor{0, 0, 0, 255};
        const CameraComponent *m_activeCamera{nullptr};
        glm::vec2 m_viewOffset{};
    };
}
