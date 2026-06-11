#pragma once
#include <string_view>
#include <glm/glm.hpp>
#include <SDL3/SDL_pixels.h>

#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"
#include "Components/SpriteTextComponent.h"
#include "UIFont.h"

namespace bomberman {
    inline bengine::SpriteTextComponent *CreateMenuLabel(bengine::Scene &scene, std::string_view text, int fontSize, const SDL_Color &color, const glm::vec2 &position) {
        auto *labelGo = scene.Add(std::make_unique<bengine::GameObject>());
        auto *textComponent = labelGo->AddComponent<bengine::SpriteTextComponent>();
        textComponent->SetFont(GetUIFont());
        textComponent->SetScale(static_cast<float>(fontSize) / 8.0f);
        textComponent->SetColor(color);
        textComponent->SetCentered(true);
        textComponent->SetIgnoreCamera(true);
        textComponent->SetText(text);
        labelGo->SetLocalPosition(position);
        return textComponent;
    }
}
