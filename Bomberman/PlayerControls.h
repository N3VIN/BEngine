#pragma once

namespace bengine {
    class GameObject;
    class InputManager;
}

namespace bomberman {
    class BombManagerComponent;

    void BindKeyboardControls(bengine::InputManager &input, bengine::GameObject *player, BombManagerComponent &bombs);
    void BindGamepadControls(bengine::InputManager &input, int controllerIndex, bengine::GameObject *player, BombManagerComponent &bombs);
}
