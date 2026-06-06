#pragma once
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace bomberman {
    class BombComponent final : public bengine::Component {
    public:
        BombComponent(bengine::GameObject *parent, glm::ivec2 cell, float fuseTime, int blastRadius);

        bool AdvanceFuse(float deltaTime);
        void MarkDetonated();

        [[nodiscard]] bool IsDetonated() const;
        [[nodiscard]] glm::ivec2 GetCell() const;
        [[nodiscard]] int GetRadius() const;
        [[nodiscard]] bengine::GameObject *GetGameObject() const;

    private:
        glm::ivec2 m_cell;
        bengine::Timer m_fuseTimer;
        int m_blastRadius;
        bool m_detonated{false};
    };
}
