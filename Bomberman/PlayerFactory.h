#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerStateComponent.h"

namespace bomberman {
    struct PlayerConfig {
        LevelGridComponent *gridComponent{};
        glm::ivec2 spawnCell{};
        float cellsPerSecond{4.0f};
    };

    inline bengine::GameObject *CreatePlayer(bengine::Scene &scene, const PlayerConfig &config) {
        auto *player = scene.Add(std::make_unique<bengine::GameObject>());
        player->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, config.cellsPerSecond);
        player->AddComponent<SpriteRendererComponent>();
        player->AddComponent<HealthComponent>(4);
        player->AddComponent<PlayerStateComponent>();

        return player;
    }
}
