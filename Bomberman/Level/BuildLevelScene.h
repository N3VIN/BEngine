#pragma once
#include <string_view>
#include <array>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "SceneGraph/Scene.h"
#include "Components/CameraComponent.h"
#include "EnemyType.h"

namespace bomberman {
    class LevelGridComponent;
    class BombManagerComponent;
    class IGameMode;
    struct Tileset;

    struct LevelSpawns {
        std::array<glm::ivec2, 2> players{};
        std::vector<std::pair<EnemyType, glm::ivec2> > enemies{};
    };

    struct LevelScene {
        bengine::Scene &scene;
        std::vector<bengine::GameObject *> players{};
    };

    LevelGridComponent *CreateLevelBackground(bengine::Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent);
    bengine::CameraComponent *CreateCamera(bengine::Scene &scene, const LevelGridComponent *gridComponent);
    LevelSpawns SpawnLevelTiles(bengine::Scene &scene, LevelGridComponent *gridComponent);
    void CreateFPSDisplay(bengine::Scene &scene);

    LevelScene BuildLevelScene(std::string_view jsonRelativePath, const IGameMode &mode);
}
