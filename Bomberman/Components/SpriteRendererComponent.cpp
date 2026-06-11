#include "SpriteRendererComponent.h"
#include "Components/RenderComponent.h"
#include "SceneGraph/GameObject.h"

bomberman::SpriteRendererComponent::SpriteRendererComponent(bengine::GameObject *parent, SpriteType spriteType)
    : bengine::Component(parent) {
    const auto &tileset = GetTileset();
    const auto definition = tileset.GetSpriteDefinition(spriteType);

    m_col = definition.col;
    m_row = definition.row;
    m_numCols = definition.numCols;
    m_numRows = definition.numRows;
    m_tileSize = tileset.tileSize;
    m_frameCount = definition.frameCount;
    m_frameColumns = definition.frameColumns > 0 ? definition.frameColumns : definition.frameCount;
    m_frameStrideCols = definition.frameStrideCols;
    m_frameStrideRows = definition.frameStrideRows;

    if (definition.autoPlay) {
        m_frameTimer = bengine::Timer{1.f / ANIMATION_FPS};
        m_playing = true;
    }

    if (!parent->HasComponent<bengine::RenderComponent>()) {
        parent->AddComponent<bengine::RenderComponent>();
    }

    m_renderComponent = parent->GetComponent<bengine::RenderComponent>();
    m_renderComponent->SetTexture(tileset.spriteTexturePath);
    m_renderComponent->SetScale(tileset.spriteScale);
    ApplySourceRect(0);
}

bomberman::SpriteRendererComponent::SpriteRendererComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {
    const auto &tileset = GetTileset();
    m_tileSize = tileset.tileSize;

    if (!parent->HasComponent<bengine::RenderComponent>()) {
        parent->AddComponent<bengine::RenderComponent>();
    }

    m_renderComponent = parent->GetComponent<bengine::RenderComponent>();
    m_renderComponent->SetTexture(tileset.spriteTexturePath);
    m_renderComponent->SetScale(tileset.spriteScale);
}

void bomberman::SpriteRendererComponent::Update(float deltaTime) {
    if (!m_playing || m_frameCount <= 1) {
        return;
    }

    m_frameTimer.Update(deltaTime);
    if (m_frameTimer.IsExpired()) {
        m_frameTimer.Reset();
        ++m_currentFrame;
        if (m_currentFrame >= m_frameCount) {
            if (m_looping) {
                m_currentFrame = 0;
            }
            else {
                m_currentFrame = m_frameCount - 1;
                m_playing = false;
            }
        }

        ApplySourceRect(m_currentFrame);
    }
}

void bomberman::SpriteRendererComponent::ApplySourceRect(int frame) const {
    const int frameRow = frame / m_frameColumns;
    const int frameCol = frame % m_frameColumns;
    const int strideCols = m_frameStrideCols > 0 ? m_frameStrideCols : m_numCols;
    const int strideRows = m_frameStrideRows > 0 ? m_frameStrideRows : m_numRows;
    const int xPos = (m_col + frameCol * strideCols) * m_tileSize;
    const int yPos = (m_row + frameRow * strideRows) * m_tileSize;
    const int width = m_numCols * m_tileSize;
    const int height = m_numRows * m_tileSize;

    m_renderComponent->SetSourceRect(SDL_Rect{xPos, yPos, width, height});
}

void bomberman::SpriteRendererComponent::PlayOnce(float fps) {
    m_frameTimer = bengine::Timer{1.f / fps};
    m_currentFrame = 0;
    m_looping = false;
    m_playing = true;
    ApplySourceRect(0);
}

void bomberman::SpriteRendererComponent::Play(const SpriteDefinition &definition, bool loop, float fps) {
    m_col = definition.col;
    m_row = definition.row;
    m_numCols = definition.numCols;
    m_numRows = definition.numRows;
    m_frameCount = definition.frameCount;
    m_frameColumns = definition.frameColumns > 0 ? definition.frameColumns : definition.frameCount;
    m_frameStrideCols = definition.frameStrideCols;
    m_frameStrideRows = definition.frameStrideRows;
    m_looping = loop;
    m_currentFrame = 0;
    m_frameTimer = bengine::Timer{1.f / fps};
    m_playing = true;
    ApplySourceRect(0);
}

void bomberman::SpriteRendererComponent::SetIgnoreCamera(bool ignore) const {
    m_renderComponent->SetIgnoreCamera(ignore);
}
