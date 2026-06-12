#pragma once
#include <cstdint>
#include <string_view>

namespace utils {
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
}
