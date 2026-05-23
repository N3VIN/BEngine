#pragma once
#include <string_view>
#include <array>
#include <glm/glm.hpp>
#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"
#include "Components/CameraComponent.h"
#include "Input/InputManager.h"

namespace bomberman {
    class LevelGridComponent;
    class BombManagerComponent;
    struct Tileset;

    LevelGridComponent *CreateLevelBackground(bengine::Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent);
    bengine::CameraComponent *CreateCamera(bengine::Scene &scene, LevelGridComponent *gridComponent);
    std::array<glm::ivec2, 2> SpawnLevelTiles(bengine::Scene &scene, LevelGridComponent *gridComponent);
    void CreateFPSDisplay(bengine::Scene &scene);
    void SetupPlayerInput(bengine::InputManager &input, bengine::GameObject *p1, bengine::GameObject *p2, BombManagerComponent *bombManager);

    bengine::Scene &BuildLevelScene(std::string_view jsonRelativePath);
}
