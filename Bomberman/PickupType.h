#pragma once
#include <cstdint>

namespace bomberman {
    enum class PickupType : uint8_t {
        BombUp,
        FlameUp,
        Detonator
    };

    inline constexpr size_t PickupTypeCount{3};
}
