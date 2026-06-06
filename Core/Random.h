#pragma once
#include <random>

namespace bengine {
    [[nodiscard]] inline std::mt19937 &RandomEngine() {
        thread_local std::mt19937 engine{std::random_device{}()};
        return engine;
    }

    [[nodiscard]] inline float RandomFloat(float min, float max) {
        return std::uniform_real_distribution{min, max}(RandomEngine());
    }

    [[nodiscard]] inline int RandomInt(int min, int max) {
        return std::uniform_int_distribution{min, max}(RandomEngine());
    }
}
