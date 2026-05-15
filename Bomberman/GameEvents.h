#pragma once
#include <glm/glm.hpp>

namespace dae {
    class GameObject;

    namespace events {
        struct ExplosionAt {
            glm::ivec2 cell{};
        };

        struct BrickDestroyed {
            glm::ivec2 cell{};
            GameObject *brick{nullptr};
        };

        struct BombDetonated {
            glm::ivec2 cell{};
        };

        struct ExplosionExpired {
            glm::ivec2 cell{};
        };

        struct PlayerDamaged {
            GameObject *player{nullptr};
            int newLives{0};
        };
    }
}
