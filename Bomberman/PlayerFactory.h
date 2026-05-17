#pragma once
#include <string_view>
#include <glm/glm.hpp>

namespace dae {
    class GameObject;
    class Scene;
    class LevelGridComponent;

    struct PlayerConfig {
        LevelGridComponent *gridComponent{};
        glm::ivec2 spawnCell{};
        std::string_view texturePath{};
        float cellsPerSecond{4.0f};
        float scale{1.0f};
    };

    GameObject *CreatePlayer(Scene &scene, const PlayerConfig &config);
}
