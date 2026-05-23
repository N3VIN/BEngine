#include "ExplosionComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

bomberman::ExplosionComponent::ExplosionComponent(bengine::GameObject *parent, glm::ivec2 cell, float lifetime)
    : bengine::Component(parent)
  , m_cell(cell)
  , m_timer(lifetime) {}

void bomberman::ExplosionComponent::Update(float deltaTime) {
    m_timer.Update(deltaTime);
    if (m_timer.IsExpired()) {
        bengine::ServiceLocator::GetEventBus().Broadcast(events::ExplosionExpired{m_cell});
    }
}

void bomberman::ExplosionComponent::ExtendLifetime(float lifetime) {
    m_timer.SetDuration(lifetime);
}
