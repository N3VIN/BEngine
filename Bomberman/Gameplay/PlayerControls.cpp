#include "PlayerControls.h"

#include "Commands/GridMoveCommand.h"
#include "Commands/BombPlaceCommand.h"
#include "Commands/DetonateCommand.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"

void bomberman::BindKeyboardControls(bengine::InputManager &input, bengine::GameObject *player, BombManagerComponent &bombs) {
    input.BindCommand(SDL_SCANCODE_W, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, -1}));
    input.BindCommand(SDL_SCANCODE_S, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, 1}));
    input.BindCommand(SDL_SCANCODE_A, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{-1, 0}));
    input.BindCommand(SDL_SCANCODE_D, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{1, 0}));

    input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, std::make_unique<BombPlaceCommand>(player, &bombs));
    input.BindCommand(SDL_SCANCODE_LSHIFT, bengine::KeyState::Down, std::make_unique<DetonateCommand>(player, &bombs));
}

void bomberman::BindGamepadControls(bengine::InputManager &input, int controllerIndex, bengine::GameObject *player, BombManagerComponent &bombs) {
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, -1}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, 1}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadLeft, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{-1, 0}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadRight, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{1, 0}));

    input.BindCommand(controllerIndex, bengine::Gamepad::Button::A, bengine::KeyState::Down, std::make_unique<BombPlaceCommand>(player, &bombs));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::B, bengine::KeyState::Down, std::make_unique<DetonateCommand>(player, &bombs));
}

void bomberman::BindGamepadMovementOnly(bengine::InputManager &input, int controllerIndex, bengine::GameObject *player) {
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, -1}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{0, 1}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadLeft, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{-1, 0}));
    input.BindCommand(controllerIndex, bengine::Gamepad::Button::DpadRight, bengine::KeyState::Pressed, std::make_unique<GridMoveCommand>(player, glm::ivec2{1, 0}));
}
