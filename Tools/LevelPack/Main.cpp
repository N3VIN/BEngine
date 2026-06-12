#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "Level/LevelData.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    const fs::path directory{argv[1]};
    if (!fs::is_directory(directory)) {
        std::cerr << "not a directory: " << directory.string() << "\n";
        return 1;
    }

    const auto writeBomb = [](const fs::path &path, const bomberman::LevelData &data) {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + path.string());
        }

        const bomberman::LevelBinaryHeader header{
            .columns = data.columns,
            .rows = data.rows,
            .cellSize = data.cellSize,
            .originX = data.origin.x,
            .originY = data.origin.y
        };

        file.write(reinterpret_cast<const char *>(&header), sizeof(header));
        for (const auto &row: data.grid) {
            file.write(row.data(), static_cast<std::streamsize>(row.size()));
        }
    };

    for (const auto &entry: fs::directory_iterator(directory)) {
        const auto &path = entry.path();
        if (path.extension() != ".json" || path.filename() == "tileset.json") {
            continue;
        }

        fs::path output = path;
        output.replace_extension(".bomb");
        writeBomb(output, bomberman::LoadLevelJson(path));
    }

    return 0;
}
