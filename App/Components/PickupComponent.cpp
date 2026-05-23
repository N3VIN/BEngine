#include "PickupComponent.h"
app::PickupComponent::PickupComponent(bengine::GameObject *gameObject)
    : bengine::Component(gameObject) {}

void app::PickupComponent::OnPickup(int value) {
    m_score += value;
    Notify(GetParent(), bengine::GameEvent::PickupPicked);
}

int app::PickupComponent::GetScore() const {
    return m_score;
}
