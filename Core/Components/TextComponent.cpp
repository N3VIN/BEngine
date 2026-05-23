#include "TextComponent.h"
#include "Renderer/Font.h"
#include "SceneGraph/GameObject.h"
#include "RenderComponent.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture2D.h"

namespace bengine {
    TextComponent::TextComponent(GameObject *owner)
        : Component(owner) {
        if (!owner->HasComponent<RenderComponent>()) {
            owner->AddComponent<RenderComponent>();
        }
    }

    void TextComponent::Update(float /*deltaTime*/) {
        if (m_needsUpdate && m_font && !m_text.empty()) {
            const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
            if (surf == nullptr) {
                throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
            }

            auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
            if (texture == nullptr) {
                throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
            }

            SDL_DestroySurface(surf);
            m_textTexture = std::make_shared<Texture2D>(texture);
            GetParent()->GetComponent<RenderComponent>()->SetTexture(m_textTexture);
            m_needsUpdate = false;
        }
    }

    void TextComponent::SetText(std::string_view text) {
        m_text = text;
        m_needsUpdate = true;
    }

    void TextComponent::SetFont(std::shared_ptr<Font> font) {
        m_font = std::move(font);
        m_needsUpdate = true;
    }

    void TextComponent::SetColor(const SDL_Color &color) {
        m_color = color;
        m_needsUpdate = true;
    }
} // bengine
