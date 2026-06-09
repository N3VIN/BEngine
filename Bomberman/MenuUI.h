#pragma once
#include <string_view>
#include <glm/glm.hpp>
#include <SDL3/SDL_pixels.h>

#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"
#include "Components/TextComponent.h"
#include "Components/RenderComponent.h"
#include "Renderer/ResourceManager.h"

namespace bomberman {
    inline bengine::TextComponent *CreateMenuLabel(bengine::Scene &scene, std::string_view text, int fontSize, const SDL_Color &color, const glm::vec2 &position) {
        auto *labelGo = scene.Add(std::make_unique<bengine::GameObject>());
        auto *textComponent = labelGo->AddComponent<bengine::TextComponent>();
        textComponent->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", static_cast<uint8_t>(fontSize)));
        textComponent->SetColor(color);
        textComponent->SetText(text);
        labelGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
        labelGo->SetLocalPosition(position);
        return textComponent;
    }
}
