#pragma once
#include <glm/glm.hpp>
#include "Renderer/Renderer.h"

namespace dae {
    inline glm::vec2 ScreenFraction(float fx, float fy) {
        const auto size = Renderer::GetInstance().GetWindowSize();
        return glm::vec2(fx * size.x, fy * size.y);
    }
}
