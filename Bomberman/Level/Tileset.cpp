#include "Tileset.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "Renderer/ResourceManager.h"

namespace dae {
    SpriteDefinition Tileset::GetSpriteDefinition(SpriteType type) const {
        SpriteDefinition definition;
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

    const Tileset &GetTileset() {
        static const Tileset tileset = [] {
            const auto path = ResourceManager::GetInstance().GetDataPath() / "Levels" / "tileset.json";

            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open tileset file: " + path.string());
            }

            nlohmann::json json;
            file >> json;

            Tileset ts;
            ts.texturePath = json.at("texture").get<std::string>();
            ts.spriteTexturePath = json.value("spriteTexture", "");
            ts.tileSize = json.at("tileSize").get<int>();
            ts.spriteScale = json.value("spriteScale", 2.0f);
            ts.explosionLifetime = json.value("explosionLifetime", 0.5f);

            {
                const auto &background = json.at("tiles").at("background");
                ts.backgroundCoord = glm::ivec2{background.at("x").get<int>(), background.at("y").get<int>()};
            }

            const auto readSpriteDefinition = [&](const char *key) {
                const auto &node = json.at("tiles").at(key);
                SpriteDefinition definition;
                definition.col = node.at("col").get<int>();
                definition.row = node.at("row").get<int>();
                definition.frameCount = node.value("frameCount", 1);
                return definition;
            };

            ts.brick = readSpriteDefinition("brick");
            ts.exit = readSpriteDefinition("exit");

            if (json.contains("bomb")) {
                const auto &node = json.at("bomb");
                ts.bomb.col = node.at("col").get<int>();
                ts.bomb.row = node.at("row").get<int>();
                ts.bomb.frameCount = node.value("frameCount", 1);
            }

            if (json.contains("explosion")) {
                const auto &node = json.at("explosion");
                ts.explosion.col = node.at("col").get<int>();
                ts.explosion.row = node.at("row").get<int>();
                ts.explosion.numCols = node.at("numCols").get<int>();
                ts.explosion.numRows = node.at("numRows").get<int>();
                ts.explosion.frameCount = node.value("frameCount", 1);
                ts.explosion.frameColumns = node.value("frameColumns", 0);
            }

            return ts;
        }();

        return tileset;
    }
}
