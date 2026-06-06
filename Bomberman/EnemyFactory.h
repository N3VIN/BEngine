#pragma once
#include <memory>
#include <span>
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "EnemyType.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/EnemyStateComponent.h"
#include "Components/EnemyAIComponent.h"

namespace bomberman {
    struct EnemyConfig {
        LevelGridComponent *gridComponent{};
        EnemyType type{};
        glm::ivec2 spawnCell{};
        std::span<bengine::GameObject *const> players{};
    };

    inline bengine::GameObject *CreateEnemy(bengine::Scene &scene, const EnemyConfig &config) {
        const auto &stats = GetEnemyStats(config.type);

        auto *enemy = scene.Add(std::make_unique<bengine::GameObject>());
        enemy->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, stats.cellsPerSecond);
        enemy->AddComponent<SpriteRendererComponent>();
        enemy->AddComponent<EnemyStateComponent>(config.type);
        enemy->AddComponent<EnemyAIComponent>(config.gridComponent, config.type, config.players);

        return enemy;
    }
}
