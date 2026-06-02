#pragma once
#include <cstdint>

namespace bomberman {
    enum class TileType : uint8_t {
        Empty,
        Wall,
        Brick,
        Exit,
        Player1Spawn,
        Player2Spawn,
        BalloomSpawn,
        OnilSpawn,
        DallSpawn,
        MinvoSpawn
    };
}
