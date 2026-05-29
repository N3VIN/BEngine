#include "PlayerFactory.h"
#include <memory>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerStateComponent.h"

bengine::GameObject *bomberman::CreatePlayer(bengine::Scene &scene, const PlayerConfig &config) {
    auto player = std::make_unique<bengine::GameObject>();
    player->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, config.cellsPerSecond);
    player->AddComponent<SpriteRendererComponent>();
    player->AddComponent<HealthComponent>(4);
    player->AddComponent<PlayerStateComponent>();
    auto *pPlayer = player.get();
    scene.Add(std::move(player));

    return pPlayer;
}
