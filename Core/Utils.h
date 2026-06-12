#pragma once
#include <cstdint>
#include <random>
#include <string_view>
#include <glm/glm.hpp>
#include "Renderer/Renderer.h"

namespace bengine {
    // i dont understand this blackmagic but a search for a simple branchless compile time hash function leads to this
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
    consteval uint32_t FNV1a(std::string_view s) {
        uint32_t h = 2166136261u;
        for (const char c: s) {
            h ^= static_cast<uint32_t>(c); // 32 bit
            h *= 16777619u;
        }

        return h;
    }

    consteval uint32_t Hash(std::string_view s) {
        return FNV1a(s);
    }

    constexpr int Wrap(int value, int modulus) {
        const int result = value % modulus;
        return result < 0 ? result + modulus : result;
    }

    namespace detail {
        [[nodiscard]] inline std::mt19937 &RandomEngine() {
            thread_local std::mt19937 engine{std::random_device{}()};
            return engine;
        }
    }

    [[nodiscard]] inline float RandomFloat(float min, float max) {
        return std::uniform_real_distribution{min, max}(detail::RandomEngine());
    }

    [[nodiscard]] inline int RandomInt(int min, int max) {
        return std::uniform_int_distribution{min, max}(detail::RandomEngine());
    }

    [[nodiscard]] inline glm::vec2 ScreenFraction(float fx, float fy) {
        const auto size = Renderer::GetInstance().GetWindowSize();
        return glm::vec2(fx * size.x, fy * size.y);
    }
}
