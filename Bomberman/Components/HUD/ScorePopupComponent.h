#pragma once
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Timer.h"

namespace bengine {
    class GameObject;
    class Scene;
}

namespace bomberman {
    static constexpr int pointSpriteX{112};
    static constexpr int pointSpriteY{336};
    static constexpr int pointSpriteWidth{16};
    static constexpr int pointSpriteHeight{8};

    [[nodiscard]] static int PointSpriteIndex(int points);

    class ScorePopupComponent final : public bengine::Component {
    public:
        ScorePopupComponent(bengine::GameObject *owner, float lifetime, float riseDistance);

        void Update(float deltaTime) override;

    private:
        bengine::Timer m_timer;
        glm::vec2 m_basePosition{};
        float m_riseDistance{};
        bool m_removed{false};
    };

    void SpawnScorePopup(bengine::Scene &scene, glm::vec2 worldPos, float cellSize, int points);
}
