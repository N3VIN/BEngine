#include "RenderComponent.h"

#include <glm/trigonometric.hpp>
#include "SceneGraph/GameObject.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "CameraComponent.h"

bengine::RenderComponent::RenderComponent(GameObject *parent)
    : Component(parent) {}

void bengine::RenderComponent::Update(float /*deltaTime*/) {}

void bengine::RenderComponent::Render() const {
    if (!m_texture) {
        return;
    }

    const auto &renderer = Renderer::GetInstance();
    auto worldPos = GetParent()->GetWorldPosition();
    float scale = m_scale;

    if (!m_ignoreCamera) {
        if (const auto camera = renderer.GetActiveCamera()) {
            worldPos = renderer.WorldToScreen(worldPos);
            scale *= camera->GetZoom();
        }
    }

    const glm::vec2 sourceSize = m_hasSrcRect ? glm::vec2{static_cast<float>(m_srcRect.w), static_cast<float>(m_srcRect.h)} : m_texture->GetSize();
    const SDL_FRect dst{worldPos.x, worldPos.y, sourceSize.x * scale, sourceSize.y * scale};
    const SDL_Rect *srcPtr = m_hasSrcRect ? &m_srcRect : nullptr;
    const float rotation = glm::degrees(GetParent()->GetWorldRotation());

    renderer.RenderTexture(*m_texture, dst, srcPtr, rotation);
}

void bengine::RenderComponent::SetTexture(std::string_view filename) {
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void bengine::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture) {
    m_texture = std::move(texture);
}

void bengine::RenderComponent::SetSourceRect(int x, int y, int w, int h) {
    m_srcRect = {x, y, w, h};
    m_hasSrcRect = true;
}

void bengine::RenderComponent::SetSourceRect(const SDL_Rect &rect) {
    m_srcRect = rect;
    m_hasSrcRect = true;
}

void bengine::RenderComponent::ClearSourceRect() {
    m_srcRect = {0, 0, 0, 0};
    m_hasSrcRect = false;
}

void bengine::RenderComponent::SetScale(float scale) {
    m_scale = scale;
}
