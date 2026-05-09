#pragma once
#include "Components/Component.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace dae {
    class BombManagerComponent;
    class LevelGridComponent;
    class GameObject;

    class BombComponent final : public Component {
    public:
        BombComponent(GameObject *parent, BombManagerComponent *bombManagerComponent, LevelGridComponent *grid, glm::ivec2 cell, GameObject *owner, float fuseTime);

        void FixedUpdate() override;

        void Detonate();

        [[nodiscard]] GameObject *GetOwner() const {
            return m_owner;
        }

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

        [[nodiscard]] GameObject *GetGameObject() const {
            return GetParent();
        }

    private:
        BombManagerComponent *m_bombManagerComponent;
        LevelGridComponent *m_grid;
        glm::ivec2 m_cell;
        GameObject *m_owner;
        Timer m_fuseTimer;
        bool m_detonated = false;
    };
}
