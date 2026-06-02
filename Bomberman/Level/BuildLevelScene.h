#pragma once
#include <string_view>
#include <array>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "SceneGraph/Scene.h"
#include "Components/CameraComponent.h"
#include "Input/InputManager.h"
#include "EnemyType.h"

namespace bomberman {
    class LevelGridComponent;
    class BombManagerComponent;
    struct Tileset;

    struct LevelSpawns {
        std::array<glm::ivec2, 2> players{};
        std::vector<std::pair<EnemyType, glm::ivec2> > enemies{};
    };

    LevelGridComponent *CreateLevelBackground(bengine::Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent);
    bengine::CameraComponent *CreateCamera(bengine::Scene &scene, const LevelGridComponent *gridComponent);
    LevelSpawns SpawnLevelTiles(bengine::Scene &scene, LevelGridComponent *gridComponent);
    void CreateFPSDisplay(bengine::Scene &scene);
    void SetupPlayerInput(bengine::InputManager &input, bengine::GameObject *p1, bengine::GameObject *p2, BombManagerComponent *bombManager);

    bengine::Scene &BuildLevelScene(std::string_view jsonRelativePath);
}
