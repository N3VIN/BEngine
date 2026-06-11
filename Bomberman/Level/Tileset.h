#pragma once
#include <string>
#include <array>
#include <glm/vec2.hpp>
#include "EnemyType.h"
#include "PickupType.h"

namespace bomberman {
    struct SpriteDefinition {
        int col{};
        int row{};
        int numCols{1};
        int numRows{1};
        int frameCount{1};
        bool autoPlay{false};
        int frameColumns{0};
        int frameStrideCols{0};
        int frameStrideRows{0};
    };

    enum class SpriteType {
        Brick,
        Exit,
        Bomb,
        Explosion
    };

    struct PlayerSprites {
        SpriteDefinition walkDown{};
        SpriteDefinition walkUp{};
        SpriteDefinition walkLeft{};
        SpriteDefinition walkRight{};
        SpriteDefinition death{};
    };

    struct EnemySprites {
        SpriteDefinition walkLeft{};
        SpriteDefinition walkRight{};
        SpriteDefinition death{};
    };

    struct ExplosionSprites {
        SpriteDefinition center{};
        SpriteDefinition armHorizontal{};
        SpriteDefinition armVertical{};
        SpriteDefinition tipUp{};
        SpriteDefinition tipDown{};
        SpriteDefinition tipLeft{};
        SpriteDefinition tipRight{};
    };

    struct Tileset {
        std::string texturePath;
        std::string spriteTexturePath;
        int tileSize{};
        float spriteScale{};
        float explosionLifetime{};
        glm::ivec2 backgroundCoord{};
        SpriteDefinition brick{};
        SpriteDefinition exit{};
        SpriteDefinition bomb{};
        SpriteDefinition explosion{};
        ExplosionSprites explosionPieces{};
        PlayerSprites player{};
        std::array<EnemySprites, EnemyTypeCount> enemies{};
        std::array<SpriteDefinition, PickupTypeCount> pickups{};

        [[nodiscard]] SpriteDefinition GetSpriteDefinition(SpriteType type) const;

        [[nodiscard]] const EnemySprites &GetEnemySprites(EnemyType type) const {
            return enemies[static_cast<size_t>(type)];
        }

        [[nodiscard]] const SpriteDefinition &GetPickupSprite(PickupType type) const {
            return pickups[static_cast<size_t>(type)];
        }
    };

    const Tileset &GetTileset();
}
