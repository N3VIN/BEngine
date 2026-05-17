#pragma once
#include <string_view>
#include <array>
#include <glm/glm.hpp>

namespace dae {
    class Scene;
    class GameObject;
    class LevelGridComponent;
    class CameraComponent;
    class BombManagerComponent;
    class InputManager;
    struct Tileset;

    LevelGridComponent *CreateLevelBackground(Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent);
    CameraComponent *CreateCamera(Scene &scene, LevelGridComponent *gridComponent);
    std::array<glm::ivec2, 2> SpawnLevelTiles(Scene &scene, LevelGridComponent *gridComponent);
    void CreateFPSDisplay(Scene &scene);
    void SetupPlayerInput(InputManager &input, GameObject *p1, GameObject *p2, BombManagerComponent *bombManager);

    Scene &BuildLevelScene(std::string_view jsonRelativePath);
}
