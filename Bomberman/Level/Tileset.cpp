#include "Tileset.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "Renderer/ResourceManager.h"

bomberman::SpriteDefinition bomberman::Tileset::GetSpriteDefinition(SpriteType type) const {
    SpriteDefinition definition;
    switch (type) {
        case SpriteType::Brick:
            definition = brick;
            break;
        case SpriteType::Exit:
            definition = exit;
            break;
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

const bomberman::Tileset &bomberman::GetTileset() {
    static const Tileset tileset = [] {
        const auto path = bengine::ResourceManager::GetInstance().GetDataPath() / "Levels" / "tileset.json";

        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open tileset file: " + path.string());
        }

        nlohmann::json json;
        file >> json;

        Tileset ts;
        ts.texturePath = json.at("texture").get<std::string>();
        ts.spriteTexturePath = json.at("spriteTexture").get<std::string>();
        ts.tileSize = json.at("tileSize").get<int>();
        ts.spriteScale = json.at("spriteScale").get<float>();
        ts.explosionLifetime = json.at("explosionLifetime").get<float>();
        ts.bombFuseDuration = json.at("bombFuseDuration").get<float>();
        ts.maxBombs = json.at("maxBombs").get<int>();
        ts.maxBlastRadius = json.at("maxBlastRadius").get<int>();
        ts.startingBombs = json.at("startingBombs").get<int>();
        ts.startingFlame = json.at("startingFlame").get<int>();
        ts.startingLives = json.at("startingLives").get<int>();
        ts.iframeDuration = json.at("iframeDuration").get<float>();
        ts.animationFps = json.at("animationFps").get<float>();
        ts.scorePopupLifetime = json.at("scorePopupLifetime").get<float>(); {
            const auto &background = json.at("tiles").at("background");
            ts.backgroundCoord = glm::ivec2{background.at("x").get<int>(), background.at("y").get<int>()};
        }

        const auto readSpriteDefinition = [&](const char *key) {
            const auto &node = json.at("tiles").at(key);
            SpriteDefinition definition;
            definition.col = node.at("col").get<int>();
            definition.row = node.at("row").get<int>();
            definition.frameCount = node.at("frameCount").get<int>();
            return definition;
        };

        ts.brick = readSpriteDefinition("brick");
        ts.exit = readSpriteDefinition("exit");

        {
            const auto &pickups = json.at("pickups");
            const auto readPickup = [&](const char *key) {
                const auto &node = pickups.at(key);
                SpriteDefinition definition;
                definition.col = node.at("col").get<int>();
                definition.row = node.at("row").get<int>();
                definition.frameCount = node.at("frameCount").get<int>();
                return definition;
            };

            ts.pickups[static_cast<size_t>(PickupType::BombUp)] = readPickup("bombUp");
            ts.pickups[static_cast<size_t>(PickupType::FlameUp)] = readPickup("flameUp");
            ts.pickups[static_cast<size_t>(PickupType::Detonator)] = readPickup("detonator");
        }

        {
            const auto &node = json.at("bomb");
            ts.bomb.col = node.at("col").get<int>();
            ts.bomb.row = node.at("row").get<int>();
            ts.bomb.frameCount = node.at("frameCount").get<int>();
        }

        {
            const auto &node = json.at("explosion");
            ts.explosion.col = node.at("col").get<int>();
            ts.explosion.row = node.at("row").get<int>();
            ts.explosion.numCols = node.at("numCols").get<int>();
            ts.explosion.numRows = node.at("numRows").get<int>();
            ts.explosion.frameCount = node.at("frameCount").get<int>();
            ts.explosion.frameColumns = node.at("frameColumns").get<int>();

            // slice the explosion into pieces
            const auto piece = [&](int defCol, int defRow) {
                SpriteDefinition definition;
                definition.col = ts.explosion.col + defCol;
                definition.row = ts.explosion.row + defRow;
                definition.frameCount = ts.explosion.frameCount;
                definition.frameColumns = ts.explosion.frameColumns;
                definition.frameStrideCols = ts.explosion.numCols;
                definition.frameStrideRows = ts.explosion.numRows;
                return definition;
            };

            const int centerCol = ts.explosion.numCols / 2;
            const int centerRow = ts.explosion.numRows / 2;
            ts.explosionPieces.center = piece(centerCol, centerRow);
            ts.explosionPieces.armHorizontal = piece(centerCol - 1, centerRow);
            ts.explosionPieces.armVertical = piece(centerCol, centerRow - 1);
            ts.explosionPieces.tipUp = piece(centerCol, 0);
            ts.explosionPieces.tipDown = piece(centerCol, ts.explosion.numRows - 1);
            ts.explosionPieces.tipLeft = piece(0, centerRow);
            ts.explosionPieces.tipRight = piece(ts.explosion.numCols - 1, centerRow);
        }

        {
            const auto &player = json.at("player");
            const auto readPlayerClip = [&](const char *key) {
                const auto &node = player.at(key);
                SpriteDefinition definition;
                definition.col = node.at("col").get<int>();
                definition.row = node.at("row").get<int>();
                definition.frameCount = node.at("frameCount").get<int>();
                return definition;
            };

            ts.player.walkDown = readPlayerClip("walkDown");
            ts.player.walkUp = readPlayerClip("walkUp");
            ts.player.walkLeft = readPlayerClip("walkLeft");
            ts.player.walkRight = readPlayerClip("walkRight");
            ts.player.death = readPlayerClip("death");
        }

        {
            const auto &enemies = json.at("enemies");
            const auto readEnemy = [&](const char *key) {
                EnemySprites sprites;
                const auto &node = enemies.at(key);
                const auto readClip = [&](const char *clipKey) {
                    const auto &clip = node.at(clipKey);
                    SpriteDefinition definition;
                    definition.col = clip.at("col").get<int>();
                    definition.row = clip.at("row").get<int>();
                    definition.frameCount = clip.at("frameCount").get<int>();
                    return definition;
                };

                sprites.walkLeft = readClip("walkLeft");
                sprites.walkRight = readClip("walkRight");
                sprites.death = readClip("death");
                return sprites;
            };

            ts.enemies[static_cast<size_t>(EnemyType::Balloom)] = readEnemy("balloom");
            ts.enemies[static_cast<size_t>(EnemyType::Onil)] = readEnemy("onil");
            ts.enemies[static_cast<size_t>(EnemyType::Dall)] = readEnemy("dall");
            ts.enemies[static_cast<size_t>(EnemyType::Minvo)] = readEnemy("minvo");
        }

        return ts;
    }();

    return tileset;
}
