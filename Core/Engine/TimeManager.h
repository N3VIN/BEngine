#pragma once
#include "Patterns/Singleton.h"

namespace bengine {
    class Time final : public Singleton<Time> {
    public:
        const float TICKS_PER_SECOND{60.f};
        const int MS_PER_FRAME = 16; // 60fps

        float deltaTime{};
        float fixedDeltaTime{1.f / TICKS_PER_SECOND};
    };
}
