#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerControllerComponent.h"
#include "Level/Tileset.h"

namespace bomberman {
    struct PlayerConfig {
        LevelGridComponent *gridComponent{};
        glm::ivec2 spawnCell{};
        float cellsPerSecond{4.0f};
    };

    inline bengine::GameObject *CreatePlayer(bengine::Scene &scene, const PlayerConfig &config, const PlayerSprites &sprites = GetTileset().player) {
        auto *player = scene.Add(std::make_unique<bengine::GameObject>());
        player->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, config.cellsPerSecond);
        player->AddComponent<SpriteRendererComponent>();
        player->AddComponent<HealthComponent>(GetTileset().startingLives);
        player->AddComponent<PlayerControllerComponent>(sprites);

        return player;
    }

    inline bengine::GameObject *CreateBallomPlayer(bengine::Scene &scene, const PlayerConfig &config) {
        const auto &enemy = GetTileset().GetEnemySprites(EnemyType::Balloom);
        const PlayerSprites ballomSprites{
            .walkDown = enemy.walkRight,
            .walkUp = enemy.walkRight,
            .walkLeft = enemy.walkLeft,
            .walkRight = enemy.walkRight,
            .death = enemy.death,
        };

        return CreatePlayer(scene, config, ballomSprites);
    }
}
