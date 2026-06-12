#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "Components/Component.h"
#include "Components/RenderComponent.h"

namespace bomberman {
    class GridRenderComponent final : public bengine::Component {
    public:
        GridRenderComponent(bengine::GameObject *parent, std::string_view texturePath, const SDL_Rect &sourceRect, float scale);

    private:
        bengine::RenderComponent *m_renderComponent{};
    };
}
