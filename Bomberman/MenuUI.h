#pragma once
#include <memory>
#include <string_view>
#include <glm/glm.hpp>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_scancode.h>

#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"
#include "Components/SpriteTextComponent.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"
#include "Commands/MenuNavigateCommand.h"
#include "Commands/MenuConfirmCommand.h"
#include "Commands/MuteCommand.h"
#include "UIFont.h"

namespace bomberman {
    inline bengine::SpriteTextComponent *CreateMenuLabel(bengine::Scene &scene, std::string_view text, int fontSize, const SDL_Color &color, const glm::vec2 &position) {
        auto *labelGo = scene.Add(std::make_unique<bengine::GameObject>());
        auto *textComponent = labelGo->AddComponent<bengine::SpriteTextComponent>();
        textComponent->SetFont(GetUIFont());
        textComponent->SetScale(static_cast<float>(fontSize) / 8.0f);
        textComponent->SetColor(color);
        textComponent->SetAlignment(bengine::TextAlign::Center);
        textComponent->SetIgnoreCamera(true);
        textComponent->SetText(text);
        labelGo->SetLocalPosition(position);
        return textComponent;
    }

    inline void BindMuteToggle(bengine::InputManager &input) {
        input.BindCommand(SDL_SCANCODE_F2, bengine::KeyState::Down, std::make_unique<MuteCommand>());
    }

    inline void BindMenuInput(bengine::InputManager &input, MenuComponent *menu) {
        BindMuteToggle(input);
        input.BindCommand(SDL_SCANCODE_W, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, -1));
        input.BindCommand(SDL_SCANCODE_S, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, 1));
        input.BindCommand(SDL_SCANCODE_RETURN, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
        input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));

        input.BindCommand(0, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, -1));
        input.BindCommand(0, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Down, std::make_unique<MenuNavigateCommand>(menu, 1));
        input.BindCommand(0, bengine::Gamepad::Button::A, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
        input.BindCommand(0, bengine::Gamepad::Button::Start, bengine::KeyState::Down, std::make_unique<MenuConfirmCommand>(menu));
    }
}
