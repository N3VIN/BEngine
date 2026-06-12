#pragma once
#include "Patterns/Singleton.h"

namespace bengine {
    class Time final : public Singleton<Time> {
    public:
        static constexpr float ticksPerSecond{60.f};
        static constexpr int msPerFrame{16}; // 60fps

        float deltaTime{};
        float fixedDeltaTime{1.f / ticksPerSecond};
    };
}
