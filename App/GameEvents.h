#pragma once

namespace dae {
    class GameObject;

    namespace events {
        struct PlayerDamaged {
            GameObject *player{nullptr};
            int newLives{0};
        };
    }
}
