#pragma once
#include <string>
#include <array>
#include <glm/vec2.hpp>
#include "Types/EnemyType.h"
#include "Types/PickupType.h"
#include "Components/SpriteDefinition.h"

namespace bomberman {
    using bengine::SpriteDefinition;

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
        float bombFuseDuration{};
        int maxBombs{};
        int maxBlastRadius{};
        int startingBombs{};
        int startingFlame{};
        int startingLives{};
        float iframeDuration{};
        float animationFps{};
        float scorePopupLifetime{};
        glm::ivec2 backgroundCoord{};
        SpriteDefinition brick{};
        SpriteDefinition exit{};
        SpriteDefinition bomb{};
        SpriteDefinition explosion{};
        ExplosionSprites explosionPieces{};
        PlayerSprites player{};
        std::array<EnemySprites, EnemyTypeCount> enemies{};
        std::array<SpriteDefinition, pickupTypeCount> pickups{};

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
