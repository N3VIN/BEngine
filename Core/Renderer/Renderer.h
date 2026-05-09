#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "Patterns/Singleton.h"

namespace dae {
    class Texture2D;
    class CameraComponent;

    class Renderer final : public Singleton<Renderer> {
    public:
        void Init(SDL_Window *window);
        void Render() const;
        void Destroy();

        void RenderTexture(const Texture2D &texture, const SDL_FRect &dst, const SDL_Rect *src, float angleDegrees) const;

        [[nodiscard]] SDL_Renderer *GetSDLRenderer() const;

        void SetActiveCamera(const CameraComponent *camera) {
            m_activeCamera = camera;
        }

        [[nodiscard]] const CameraComponent *GetActiveCamera() const {
            return m_activeCamera;
        }

        [[nodiscard]] glm::vec2 GetWindowSize() const;
        [[nodiscard]] glm::vec2 WorldToScreen(const glm::vec2 &worldPos) const;

        [[nodiscard]] const SDL_Color &GetBackgroundColor() const {
            return m_clearColor;
        }

        void SetBackgroundColor(const SDL_Color &color) {
            m_clearColor = color;
        }

    private:
        SDL_Renderer *m_renderer{};
        SDL_Window *m_window{};
        SDL_Color m_clearColor{};
        const CameraComponent *m_activeCamera{nullptr};
    };
}
