#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "Components/Component.h"

namespace dae {
    class RenderComponent;

    class GridRenderComponent final : public Component {
    public:
        GridRenderComponent(GameObject *parent, std::string_view texturePath, const SDL_Rect &sourceRect, float scale);

    private:
        RenderComponent *m_renderComponent{};
    };
}
