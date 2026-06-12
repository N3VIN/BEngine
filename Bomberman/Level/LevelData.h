#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace bomberman {
    struct LevelData {
        int columns{};
        int rows{};
        float cellSize{};
        glm::vec2 origin{};
        std::vector<std::string> grid{};
    };

#pragma pack(push, 1) // no padding
    struct LevelBinaryHeader {
        char header[4]{'B', 'O', 'M', 'B'};
        int32_t columns{};
        int32_t rows{};
        float cellSize{};
        float originX{};
        float originY{};
    };
#pragma pack(pop)

    LevelData LoadLevelJson(const fs::path &path);
    LevelData LoadLevelBinary(const fs::path &path);
    LevelData LoadLevel(const fs::path &jsonPath);
}
