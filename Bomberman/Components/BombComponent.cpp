#include "BombComponent.h"
#include "BombManagerComponent.h"
#include "Engine/TimeManager.h"

namespace dae {
    BombComponent::BombComponent(GameObject *parent, BombManagerComponent *bombManagerComponent, LevelGridComponent *grid, glm::ivec2 cell, GameObject *owner, float fuseTime)
        : Component(parent)
      , m_bombManagerComponent(bombManagerComponent)
      , m_grid(grid)
      , m_cell(cell)
      , m_owner(owner)
      , m_fuseTimer(fuseTime) {}

    void BombComponent::FixedUpdate() {
        if (m_detonated) {
            return;
        }

        m_fuseTimer.Update(Time::GetInstance().fixedDeltaTime);
        if (m_fuseTimer.IsExpired()) {
            Detonate();
        }
    }

    void BombComponent::Detonate() {
        if (m_detonated) {
            return;
        }

        m_detonated = true;
        m_bombManagerComponent->DetonateBomb(this);
    }
}
