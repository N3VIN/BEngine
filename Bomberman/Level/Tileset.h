#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
#include <glm/vec2.hpp>
#include <nlohmann/json.hpp>

#include "Renderer/ResourceManager.h"

namespace dae {
    struct SpriteDef {
        int col{};
        int row{};
        int numCols{1};
        int numRows{1};
        int frameCount{1};
        bool autoPlay{false};
        int frameColumns{0};
    };

    enum class SpriteType {
        Brick,
        Exit,
        Bomb,
        Explosion
    };

    struct Tileset {
        std::string texturePath;
        std::string spriteTexturePath;
        int tileSize{};
        float spriteScale{};
        float explosionLifetime{};
        glm::ivec2 backgroundCoord{};
        SpriteDef brick{};
        SpriteDef exit{};
        SpriteDef bomb{};
        SpriteDef explosion{};

        [[nodiscard]] SpriteDef GetSpriteDefinition(SpriteType type) const {
            SpriteDef definition;
            switch (type) {
                case SpriteType::Brick: definition = brick; break;
                case SpriteType::Exit: definition = exit; break;
                case SpriteType::Bomb:
                    definition = bomb;
                    definition.autoPlay = true;
                    break;
                case SpriteType::Explosion:
                    definition = explosion;
                    definition.autoPlay = true;
                    break;
            }
            return definition;
        }
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

            Tileset tileset;
            tileset.texturePath = json.at("texture").get<std::string>();
            tileset.spriteTexturePath = json.value("spriteTexture", "");
            tileset.tileSize = json.at("tileSize").get<int>();
            tileset.spriteScale = json.value("spriteScale", 2.0f);
            tileset.explosionLifetime = json.value("explosionLifetime", 0.5f);

            {
                const auto &bg = json.at("tiles").at("background");
                tileset.backgroundCoord = glm::ivec2{bg.at("x").get<int>(), bg.at("y").get<int>()};
            }

            const auto readSpriteDef = [&](const char *key) {
                const auto &node = json.at("tiles").at(key);
                SpriteDef def;
                def.col = node.at("col").get<int>();
                def.row = node.at("row").get<int>();
                def.frameCount = node.value("frameCount", 1);
                return def;
            };

            tileset.brick = readSpriteDef("brick");
            tileset.exit = readSpriteDef("exit");

            if (json.contains("bomb")) {
                const auto &node = json.at("bomb");
                tileset.bomb.col = node.at("col").get<int>();
                tileset.bomb.row = node.at("row").get<int>();
                tileset.bomb.frameCount = node.value("frameCount", 1);
            }

            if (json.contains("explosion")) {
                const auto &node = json.at("explosion");
                tileset.explosion.col = node.at("col").get<int>();
                tileset.explosion.row = node.at("row").get<int>();
                tileset.explosion.numCols = node.at("numCols").get<int>();
                tileset.explosion.numRows = node.at("numRows").get<int>();
                tileset.explosion.frameCount = node.value("frameCount", 1);
                tileset.explosion.frameColumns = node.value("frameColumns", 0);
            }

            return tileset;
        }();

        return tileset;
    }
}
