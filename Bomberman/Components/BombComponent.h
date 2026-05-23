#pragma once
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace bomberman {
    class BombComponent final : public bengine::Component {
    public:
        BombComponent(bengine::GameObject *parent, glm::ivec2 cell, float fuseTime);

        void Update(float deltaTime) override;
        void Detonate();

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

        [[nodiscard]] bengine::GameObject *GetGameObject() const {
            return GetParent();
        }

    private:
        glm::ivec2 m_cell;
        bengine::Timer m_fuseTimer;
        bool m_detonated{false};
    };
}
