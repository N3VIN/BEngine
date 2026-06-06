#pragma once
#include <glm/glm.hpp>
#include "SceneGraph/GameObject.h"

namespace bomberman {
    namespace events {
        struct ExplosionAt {
            glm::ivec2 cell{};
        };

        struct BrickDestroyed {
            glm::ivec2 cell{};
            bengine::GameObject *brick{nullptr};
        };

        struct BombDetonated {
            glm::ivec2 cell{};
        };

        struct ExplosionExpired {
            glm::ivec2 cell{};
        };

        struct PlayerDamaged {
            bengine::GameObject *player{nullptr};
            int newLives{0};
        };

        struct PlayerDied {
            bengine::GameObject *player{nullptr};
        };

        struct EnemyKilled {
            bengine::GameObject *enemy{nullptr};
            glm::ivec2 cell{};
            int points{0};
        };
    }
}
