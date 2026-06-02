#include "EnemyFactory.h"
#include <memory>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Level/Tileset.h"

bengine::GameObject *bomberman::CreateEnemy(bengine::Scene &scene, LevelGridComponent *gridComponent, EnemyType type, glm::ivec2 cell) {
    const auto &stats = GetEnemyStats(type);
    const auto &sprites = GetTileset().GetEnemySprites(type);

    auto enemy = std::make_unique<bengine::GameObject>();
    enemy->AddComponent<GridMovementComponent>(gridComponent, cell, stats.cellsPerSecond);
    auto *sprite = enemy->AddComponent<SpriteRendererComponent>();
    sprite->Play(sprites.death, true);

    auto *pEnemy = enemy.get();
    scene.Add(std::move(enemy));

    return pEnemy;
}
