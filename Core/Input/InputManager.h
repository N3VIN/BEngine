#pragma once
#include "Patterns/Singleton.h"
#include "Patterns/ICommand.h"
#include "Gamepad.h"

#include <SDL3/SDL_scancode.h>
#include <memory>
#include <vector>
#include <set>
#include <array>

namespace bengine {
    enum class KeyState {
        Down,
        Up,
        Pressed
    };

    class InputManager final : public Singleton<InputManager> {
    public:
        bool ProcessInput();

        void BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<ICommand> command);
        void UnbindCommand(SDL_Scancode key, KeyState state);

        void BindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state, std::unique_ptr<ICommand> command);
        void UnbindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state);

        void UnbindAll();

    private:
        struct KeyboardBinding {
            SDL_Scancode key;
            KeyState state;
            std::unique_ptr<ICommand> command;
        };

        struct ControllerBinding {
            unsigned int controllerIndex;
            Gamepad::Button button;
            KeyState state;
            std::unique_ptr<ICommand> command;
        };

        std::vector<KeyboardBinding> m_keyboardBindings;
        std::vector<ControllerBinding> m_controllerBindings;
        std::array<Gamepad, 4> m_gamepads{Gamepad{0}, Gamepad{1}, Gamepad{2}, Gamepad{3}};

        std::set<SDL_Scancode> m_keysDown;
        std::set<SDL_Scancode> m_keysUp;
    };
}
