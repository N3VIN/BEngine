#include "LevelData.h"

#include <fstream>
#include <stdexcept>
#include <string_view>
#include <nlohmann/json.hpp>

bomberman::LevelData bomberman::LoadLevelJson(const fs::path &path) {
    if (!fs::exists(path)) {
        throw std::runtime_error("Level file not found: " + path.string());
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file: " + path.string());
    }

    nlohmann::json json;
    file >> json;

    const auto &grid = json.at("grid");
    const auto &origin = json.at("origin");
    LevelData data{
        .columns = grid.at("columns").get<int>(),
        .rows = grid.at("rows").get<int>(),
        .cellSize = grid.at("cellSize").get<float>(),
        .origin = {origin.at("x").get<float>(), origin.at("y").get<float>()},
    };

    const auto &tiles = json.at("tiles");
    data.grid.reserve(tiles.size());
    for (const auto &row: tiles) {
        data.grid.push_back(row.get<std::string>());
    }

    return data;
}

bomberman::LevelData bomberman::LoadLevelBinary(const fs::path &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file: " + path.string());
    }

    LevelBinaryHeader header{};
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (std::string_view(header.magic, sizeof(header.magic)) != "BOMB") {
        throw std::runtime_error("Invalid level binary: " + path.string()); // wrong file
    }

    LevelData data{
        .columns = header.columns,
        .rows = header.rows,
        .cellSize = header.cellSize,
        .origin = {header.originX, header.originY},
    };

    data.grid.reserve(static_cast<size_t>(header.rows));
    for (int32_t row = 0; row < header.rows; ++row) {
        std::string line(static_cast<size_t>(header.columns), '\0');
        file.read(line.data(), header.columns);
        data.grid.push_back(std::move(line));
    }

    return data;
}

bomberman::LevelData bomberman::LoadLevel(const fs::path &jsonPath) {
#ifdef NDEBUG
    fs::path bomb = jsonPath;
    bomb.replace_extension(".bomb");
    if (fs::exists(bomb)) {
        return LoadLevelBinary(bomb);
    }
#endif
    return LoadLevelJson(jsonPath);
}
