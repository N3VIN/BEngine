#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput() {
    m_keysDown.clear();
    m_keysUp.clear();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
            m_keysDown.insert(e.key.scancode);
        }
        if (e.type == SDL_EVENT_KEY_UP) {
            m_keysUp.insert(e.key.scancode);
        }

        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    for (auto &gamepad: m_gamepads) {
        gamepad.Update();
    }

    const bool *keyboardState = SDL_GetKeyboardState(nullptr);

    for (const auto &binding: m_keyboardBindings) {
        bool shouldExecute = false;
        switch (binding.state) {
            case KeyState::Down:
                shouldExecute = m_keysDown.contains(binding.key);
                break;
            case KeyState::Up:
                shouldExecute = m_keysUp.contains(binding.key);
                break;
            case KeyState::Pressed:
                shouldExecute = keyboardState[binding.key] != 0;
                break;
        }

        if (shouldExecute) {
            binding.command->Execute();
        }
    }

    for (const auto &binding: m_controllerBindings) {
        if (binding.controllerIndex >= m_gamepads.size()) {
            continue;
        }

        const Gamepad &pad = m_gamepads[binding.controllerIndex];
        bool shouldExecute = false;
        switch (binding.state) {
            case KeyState::Down:
                shouldExecute = pad.IsDown(binding.button);
                break;
            case KeyState::Up:
                shouldExecute = pad.IsUp(binding.button);
                break;
            case KeyState::Pressed:
                shouldExecute = pad.IsPressed(binding.button);
                break;
        }

        if (shouldExecute) {
            binding.command->Execute();
        }
    }

    return true;
}

void dae::InputManager::BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<ICommand> command) {
    m_keyboardBindings.push_back({key, state, std::move(command)});
}

void dae::InputManager::UnbindCommand(SDL_Scancode key, KeyState state) {
    std::erase_if(m_keyboardBindings, [key, state](const KeyboardBinding &b) {
                      return b.key == key && b.state == state;
                  }
    );
}

void dae::InputManager::BindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state, std::unique_ptr<ICommand> command) {
    m_controllerBindings.push_back({controllerIndex, button, state, std::move(command)});
}

void dae::InputManager::UnbindCommand(unsigned int controllerIndex, Gamepad::Button button, KeyState state) {
    std::erase_if(m_controllerBindings, [controllerIndex, button, state](const ControllerBinding &b) {
                      return b.controllerIndex == controllerIndex && b.button == button && b.state == state;
                  }
    );
}

void dae::InputManager::UnbindAll() {
    m_keyboardBindings.clear();
    m_controllerBindings.clear();
}
