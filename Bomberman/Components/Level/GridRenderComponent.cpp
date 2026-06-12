#include "GridRenderComponent.h"
#include "Components/RenderComponent.h"
#include "SceneGraph/GameObject.h"

bomberman::GridRenderComponent::GridRenderComponent(bengine::GameObject *parent, std::string_view texturePath, const SDL_Rect &sourceRect, float scale)
    : bengine::Component(parent) {
    if (!parent->HasComponent<bengine::RenderComponent>()) {
        parent->AddComponent<bengine::RenderComponent>();
    }

    m_renderComponent = parent->GetComponent<bengine::RenderComponent>();
    m_renderComponent->SetTexture(texturePath);
    m_renderComponent->SetSourceRect(sourceRect);
    m_renderComponent->SetScale(scale);
}
