#pragma once
#include <optional>
#include "Components/SpriteRendererComponent.h"
#include "SceneGraph/GameObject.h"
#include "Level/Tileset.h"

namespace bomberman {
    // we do this so we can use the tileset
    inline bengine::SpriteRendererComponent *AddSprite(bengine::GameObject &object, std::optional<SpriteType> type = std::nullopt) {
        auto *sprite = object.AddComponent<bengine::SpriteRendererComponent>();
        const auto &tileset = GetTileset();
        sprite->SetSpriteSheet(tileset.spriteTexturePath, tileset.spriteScale, tileset.tileSize);
        if (type) {
            sprite->SetSprite(tileset.GetSpriteDefinition(*type), tileset.animationFps);
        }

        return sprite;
    }
}
