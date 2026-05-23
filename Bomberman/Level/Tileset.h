#pragma once
#include <string>
#include <glm/vec2.hpp>

namespace bomberman {
    struct SpriteDefinition {
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
        SpriteDefinition brick{};
        SpriteDefinition exit{};
        SpriteDefinition bomb{};
        SpriteDefinition explosion{};

        [[nodiscard]] SpriteDefinition GetSpriteDefinition(SpriteType type) const;
    };

    const Tileset &GetTileset();
}
