#include "SpriteTextComponent.h"

#include <SDL3/SDL_render.h>

#include "CameraComponent.h"
#include "SceneGraph/GameObject.h"
#include "Renderer/Renderer.h"
#include "Renderer/SpriteFont.h"
#include "Renderer/Texture2D.h"

bengine::SpriteTextComponent::SpriteTextComponent(GameObject *owner)
    : Component(owner) {}

void bengine::SpriteTextComponent::Render() const {
    if (!m_font || m_text.empty()) {
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

    const auto glyphSize = m_font->GlyphSize();
    const float glyphWidth = static_cast<float>(glyphSize.x) * scale;
    const float glyphHeight = static_cast<float>(glyphSize.y) * scale;

    float positionX = worldPos.x;
    if (m_centered) {
        positionX -= 0.5f * static_cast<float>(m_text.size()) * glyphWidth;
    }

    const auto &atlas = m_font->GetAtlas();
    auto *sdlTexture = atlas.GetSDLTexture();
    SDL_SetTextureColorMod(sdlTexture, m_color.r, m_color.g, m_color.b);
    SDL_SetTextureAlphaMod(sdlTexture, m_color.a);

    for (const char character: m_text) {
        if (const auto *source = m_font->TryGetGlyph(character)) {
            const auto destination{positionX, worldPos.y, glyphWidth, glyphHeight};
            renderer.RenderTexture(atlas, destination, source, 0.0f);
        }

        positionX += glyphWidth;
    }

    SDL_SetTextureColorMod(sdlTexture, 255, 255, 255);
    SDL_SetTextureAlphaMod(sdlTexture, 255);
}

void bengine::SpriteTextComponent::SetText(std::string_view text) {
    m_text = text;
}

void bengine::SpriteTextComponent::SetFont(std::shared_ptr<SpriteFont> font) {
    m_font = std::move(font);
}

void bengine::SpriteTextComponent::SetColor(const SDL_Color &color) {
    m_color = color;
}

void bengine::SpriteTextComponent::SetScale(float scale) {
    m_scale = scale;
}

void bengine::SpriteTextComponent::SetIgnoreCamera(bool ignore) {
    m_ignoreCamera = ignore;
}

void bengine::SpriteTextComponent::SetCentered(bool centered) {
    m_centered = centered;
}
