#pragma once
#include <string_view>
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"

namespace bomberman {
    class LevelGridComponent;

    struct PlayerConfig {
        LevelGridComponent *gridComponent{};
        glm::ivec2 spawnCell{};
        std::string_view texturePath{};
        float cellsPerSecond{4.0f};
        float scale{1.0f};
    };

    bengine::GameObject *CreatePlayer(bengine::Scene &scene, const PlayerConfig &config);
}
