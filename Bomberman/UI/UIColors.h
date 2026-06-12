#pragma once
#include <SDL3/SDL_pixels.h>

namespace bomberman::colors {
    inline constexpr SDL_Color white{255, 255, 255, 255};
    inline constexpr SDL_Color menuItem{180, 180, 180, 255};
    inline constexpr SDL_Color highlight{255, 255, 80, 255};
    inline constexpr SDL_Color prompt{200, 200, 200, 255};
    inline constexpr SDL_Color scoreEntry{220, 220, 220, 255};
    inline constexpr SDL_Color hint{160, 160, 160, 255};
    inline constexpr SDL_Color danger{255, 80, 80, 255};
    inline constexpr SDL_Color debug{0, 255, 0, 255};
}
