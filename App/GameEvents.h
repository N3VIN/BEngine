#pragma once
#include "SceneGraph/GameObject.h"

namespace app {
    namespace events {
        struct PlayerDamaged {
            bengine::GameObject *player{nullptr};
            int newLives{0};
        };
    }
}
