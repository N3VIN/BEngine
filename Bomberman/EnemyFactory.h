#pragma once
#include <glm/glm.hpp>
#include "EnemyType.h"

namespace bengine {
    class GameObject;
    class Scene;
}

namespace bomberman {
    class LevelGridComponent;

    bengine::GameObject *CreateEnemy(bengine::Scene &scene, LevelGridComponent *gridComponent, EnemyType type, glm::ivec2 cell);
}
