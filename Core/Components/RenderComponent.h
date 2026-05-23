#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include "Component.h"

namespace bengine {
    class Texture2D;
    class GameObject;

    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject *parent);
        void Update(float deltaTime) override;
        void Render() const override;

        void SetTexture(std::string_view filename);
        void SetTexture(std::shared_ptr<Texture2D> texture);
        void SetSourceRect(int x, int y, int w, int h);
        void SetSourceRect(const SDL_Rect &rect);
        void ClearSourceRect();
        void SetScale(float scale);

        void SetIgnoreCamera(bool ignore) {
            m_ignoreCamera = ignore;
        }

    private:
        std::shared_ptr<Texture2D> m_texture{};
        SDL_Rect m_srcRect{0, 0, 0, 0};
        bool m_hasSrcRect{false};
        float m_scale{1.f};
        bool m_ignoreCamera{false};
    };
}
