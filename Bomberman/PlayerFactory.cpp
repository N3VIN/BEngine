#include "PlayerFactory.h"
#include <memory>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/RenderComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/HealthComponent.h"

bengine::GameObject *bomberman::CreatePlayer(bengine::Scene &scene, const PlayerConfig &config) {
    auto player = std::make_unique<bengine::GameObject>();
    auto *renderComponent = player->AddComponent<bengine::RenderComponent>();
    renderComponent->SetTexture(config.texturePath);
    renderComponent->SetScale(config.scale);
    player->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, config.cellsPerSecond);
    player->AddComponent<HealthComponent>(4);
    auto *pPlayer = player.get();
    scene.Add(std::move(player));

    return pPlayer;
}
