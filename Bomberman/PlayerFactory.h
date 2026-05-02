#pragma once
#include <memory>
#include <string_view>
#include <glm/glm.hpp>
#include "../Core/SceneGraph/GameObject.h"
#include "../Core/SceneGraph/Scene.h"
#include "../Core/Components/RenderComponent.h"
#include "Components/GridMovementComponent.h"

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
        auto *ptr = player.get();
        scene.Add(std::move(player));

        return ptr;
    }
}
