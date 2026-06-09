#pragma once
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"
#include "PickupType.h"

namespace bomberman {
    namespace events {
        struct ExplosionAt {
            glm::ivec2 cell{};
        };

        struct BrickDestroyed {
            glm::ivec2 cell{};
        };

        struct BombDetonated {
            glm::ivec2 cell{};
        };

        struct PlayerDied {
            bengine::GameObject *player{nullptr};
        };

        struct EnemyKilled {
            bengine::GameObject *enemy{nullptr};
            glm::ivec2 cell{};
            int points{0};
        };

        struct AllEnemiesDefeated {};

        struct LevelCompleted {};

        struct PickupCollected {
            bengine::GameObject *player{nullptr};
            PickupType type{};
        };
    }
}