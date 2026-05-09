#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
#include <glm/vec2.hpp>
#include <nlohmann/json.hpp>

#include "Renderer/ResourceManager.h"

namespace dae {
    struct Tileset {
        std::string texturePath;
        std::string bombTexturePath;
        int tileSize{};
        float spriteScale{};
        float explosionLifetime{};
        glm::ivec2 backgroundCoord{};
        glm::ivec2 brickCoord{};
        glm::ivec2 exitCoord{};
        glm::ivec2 bombCoord{};
        glm::ivec2 explosionCenter{};
        int explosionCells{};
    };

    inline const Tileset &GetTileset() {
        static const Tileset tileset = [] {
            const auto path = ResourceManager::GetInstance().GetDataPath() / "Levels" / "tileset.json";

            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open tileset file: " + path.string());
            }

            nlohmann::json json;
            file >> json;

            const auto readCoord = [&](const char *key) {
                const auto &node = json.at("tiles").at(key);
                return glm::ivec2{node.at("x").get<int>(), node.at("y").get<int>()};
            };

            Tileset t;
            t.texturePath = json.at("texture").get<std::string>();
            t.bombTexturePath = json.value("bombTexture", "");
            t.tileSize = json.at("tileSize").get<int>();
            t.spriteScale = json.value("spriteScale", 2.0f);
            t.explosionLifetime = json.value("explosionLifetime", 0.5f);
            t.backgroundCoord = readCoord("background");
            t.brickCoord = readCoord("brick");
            t.exitCoord = readCoord("exit");

            if (json.contains("bomb")) {
                const auto &bomb = json.at("bomb");
                t.bombCoord = glm::ivec2{bomb.at("col").get<int>(), bomb.at("row").get<int>()};
            }

            if (json.contains("explosion")) {
                const auto &explosion = json.at("explosion");
                t.explosionCenter = glm::ivec2{explosion.at("centerCol").get<int>(), explosion.at("centerRow").get<int>()};
                t.explosionCells = explosion.at("sizeCells").get<int>();
            }

            return t;
        }();

        return tileset;
    }
}
