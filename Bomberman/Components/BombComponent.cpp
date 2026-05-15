#include "BombComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

namespace dae {
    BombComponent::BombComponent(GameObject *parent, glm::ivec2 cell, float fuseTime)
        : Component(parent)
      , m_cell(cell)
      , m_fuseTimer(fuseTime) {}

    void BombComponent::Update(float deltaTime) {
        if (m_detonated) {
            return;
        }

        m_fuseTimer.Update(deltaTime);
        if (m_fuseTimer.IsExpired()) {
            Detonate();
        }
    }

    void BombComponent::Detonate() {
        if (m_detonated) {
            return;
        }

        ServiceLocator::GetEventBus().Broadcast(events::BombDetonated{m_cell});
        m_detonated = true;
    }
}
