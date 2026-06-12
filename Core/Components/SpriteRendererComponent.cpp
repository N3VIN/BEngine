#include "SpriteRendererComponent.h"
#include "Components/RenderComponent.h"
#include "SceneGraph/GameObject.h"

bengine::SpriteRendererComponent::SpriteRendererComponent(GameObject *parent)
    : Component(parent) {
    if (!parent->HasComponent<RenderComponent>()) {
        parent->AddComponent<RenderComponent>();
    }

    m_renderComponent = parent->GetComponent<RenderComponent>();
}

void bengine::SpriteRendererComponent::SetSpriteSheet(const std::string &texturePath, float scale, int tileSize) {
    m_tileSize = tileSize;
    m_renderComponent->SetTexture(texturePath);
    m_renderComponent->SetScale(scale);
}

void bengine::SpriteRendererComponent::SetSprite(const SpriteDefinition &definition, float fps) {
    m_definition = definition;

    if (definition.autoPlay) {
        m_frameTimer = Timer{1.f / fps};
        m_playing = true;
    }

    ApplySourceRect(0);
}

void bengine::SpriteRendererComponent::Update(float deltaTime) {
    if (!m_playing || m_definition.frameCount <= 1) {
        return;
    }

    m_frameTimer.Update(deltaTime);
    if (m_frameTimer.IsExpired()) {
        m_frameTimer.Reset();
        ++m_currentFrame;
        if (m_currentFrame >= m_definition.frameCount) {
            if (m_looping) {
                m_currentFrame = 0;
            }
            else {
                m_currentFrame = m_definition.frameCount - 1;
                m_playing = false;
            }
        }

        ApplySourceRect(m_currentFrame);
    }
}

void bengine::SpriteRendererComponent::ApplySourceRect(int frame) const {
    const int frameColumns = m_definition.frameColumns > 0 ? m_definition.frameColumns : m_definition.frameCount;
    const int frameRow = frame / frameColumns;
    const int frameCol = frame % frameColumns;
    const int strideCols = m_definition.frameStrideCols > 0 ? m_definition.frameStrideCols : m_definition.numCols;
    const int strideRows = m_definition.frameStrideRows > 0 ? m_definition.frameStrideRows : m_definition.numRows;
    const int xPos = (m_definition.col + frameCol * strideCols) * m_tileSize;
    const int yPos = (m_definition.row + frameRow * strideRows) * m_tileSize;
    const int width = m_definition.numCols * m_tileSize;
    const int height = m_definition.numRows * m_tileSize;

    m_renderComponent->SetSourceRect(SDL_Rect{xPos, yPos, width, height});
}

void bengine::SpriteRendererComponent::PlayOnce(float fps) {
    m_frameTimer = Timer{1.f / fps};
    m_currentFrame = 0;
    m_looping = false;
    m_playing = true;
    ApplySourceRect(0);
}

void bengine::SpriteRendererComponent::Play(const SpriteDefinition &definition, bool loop, float fps) {
    m_definition = definition;
    m_looping = loop;
    m_currentFrame = 0;
    m_frameTimer = Timer{1.f / fps};
    m_playing = true;
    ApplySourceRect(0);
}

void bengine::SpriteRendererComponent::SetIgnoreCamera(bool ignore) const {
    m_renderComponent->SetIgnoreCamera(ignore);
}
