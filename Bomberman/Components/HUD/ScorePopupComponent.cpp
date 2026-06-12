#include "ScorePopupComponent.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/RenderComponent.h"
#include "Level/Tileset.h"

bomberman::ScorePopupComponent::ScorePopupComponent(bengine::GameObject *owner, float lifetime, float riseDistance)
    : bengine::Component(owner)
  , m_timer(lifetime)
  , m_basePosition(owner->GetLocalPosition().x, owner->GetLocalPosition().y)
  , m_riseDistance(riseDistance) {}

void bomberman::ScorePopupComponent::Update(float deltaTime) {
    m_timer.Update(deltaTime);
    GetParent()->SetLocalPosition(m_basePosition - glm::vec2{0.f, m_riseDistance * m_timer.GetProgress()});

    if (m_timer.IsExpired() && !m_removed) {
        m_removed = true;
        bengine::GetActiveScene()->Remove(GetParent());
    }
}

void bomberman::SpawnScorePopup(bengine::Scene &scene, glm::vec2 worldPos, float cellSize, int points) {
    const auto pointSpriteIndex = [](int value) {
        switch (value) {
            case 100:
                return 0;
            case 200:
                return 1;
            case 400:
                return 2;
            case 800:
                return 3;
            default:
                return -1;
        }
    };

    const int index = pointSpriteIndex(points);
    if (index < 0) {
        return;
    }

    const auto &tileset = GetTileset();
    const float lifetime = tileset.scorePopupLifetime;

    auto popup = std::make_unique<bengine::GameObject>();
    auto *render = popup->AddComponent<bengine::RenderComponent>();
    render->SetTexture(tileset.spriteTexturePath);
    render->SetSourceRect(pointSpriteX, pointSpriteY + index * pointSpriteHeight, pointSpriteWidth, pointSpriteHeight);
    render->SetScale(tileset.spriteScale);

    const float spriteWidth = static_cast<float>(pointSpriteWidth) * tileset.spriteScale;
    popup->SetLocalPosition(worldPos + glm::vec2{(cellSize - spriteWidth) * 0.5f, -cellSize * 0.5f});
    popup->AddComponent<ScorePopupComponent>(lifetime, cellSize * 0.5f);

    scene.Add(std::move(popup));
}
