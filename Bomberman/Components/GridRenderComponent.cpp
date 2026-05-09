#include "GridRenderComponent.h"
#include "Components/RenderComponent.h"
#include "SceneGraph/GameObject.h"

namespace dae {
    GridRenderComponent::GridRenderComponent(GameObject *parent, std::string_view texturePath, const SDL_Rect &sourceRect, float scale)
        : Component(parent) {
        if (!parent->HasComponent<RenderComponent>()) {
            parent->AddComponent<RenderComponent>();
        }

        m_renderComponent = parent->GetComponent<RenderComponent>();
        m_renderComponent->SetTexture(texturePath);
        m_renderComponent->SetSourceRect(sourceRect);
        m_renderComponent->SetScale(scale);
    }
}
