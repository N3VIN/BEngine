#pragma once
#include "Components/Component.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace dae {
    class GameObject;

    class BombComponent final : public Component {
    public:
        BombComponent(GameObject *parent, glm::ivec2 cell, float fuseTime);

        void Update(float deltaTime) override;
        void Detonate();

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

        [[nodiscard]] GameObject *GetGameObject() const {
            return GetParent();
        }

    private:
        glm::ivec2 m_cell;
        Timer m_fuseTimer;
        bool m_detonated{false};
    };
}
