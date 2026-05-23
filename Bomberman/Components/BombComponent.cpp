#include "BombComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

bomberman::BombComponent::BombComponent(bengine::GameObject *parent, glm::ivec2 cell, float fuseTime)
    : bengine::Component(parent)
  , m_cell(cell)
  , m_fuseTimer(fuseTime) {}

void bomberman::BombComponent::Update(float deltaTime) {
    if (m_detonated) {
        return;
    }

    m_fuseTimer.Update(deltaTime);
    if (m_fuseTimer.IsExpired()) {
        Detonate();
    }
}

void bomberman::BombComponent::Detonate() {
    if (m_detonated) {
        return;
    }

    bengine::ServiceLocator::GetEventBus().Broadcast(events::BombDetonated{m_cell});
    m_detonated = true;
}
