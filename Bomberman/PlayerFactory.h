#pragma once
#include <memory>
#include <string_view>
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Components/RenderComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/HealthComponent.h"

namespace dae {
    class LevelGridComponent;

    struct PlayerConfig {
        LevelGridComponent *gridComponent{};
        glm::ivec2 spawnCell{};
        std::string_view texturePath{};
        float cellsPerSecond{4.0f};
        float scale{1.0f};
    };

    inline GameObject *CreatePlayer(Scene &scene, const PlayerConfig &config) {
        auto player = std::make_unique<GameObject>();
        auto *renderComponent = player->AddComponent<RenderComponent>();
        renderComponent->SetTexture(config.texturePath);
        renderComponent->SetScale(config.scale);
        player->AddComponent<GridMovementComponent>(config.gridComponent, config.spawnCell, config.cellsPerSecond);
        player->AddComponent<HealthComponent>(4);
        auto *ptr = player.get();
        scene.Add(std::move(player));

        return ptr;
    }
}
