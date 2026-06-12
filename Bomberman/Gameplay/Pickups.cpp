#include "Pickups.h"
#include "Level/Tileset.h"

bomberman::Pickup::Pickup(glm::ivec2 cell)
    : m_cell(cell) {}

glm::ivec2 bomberman::Pickup::GetCell() const {
    return m_cell;
}

bool bomberman::Pickup::IsRevealed() const {
    return m_revealed;
}

bengine::GameObject *bomberman::Pickup::GetObject() const {
    return m_object;
}

void bomberman::Pickup::MarkRevealed(bengine::GameObject *object) {
    m_revealed = true;
    m_object = object;
}

const bomberman::SpriteDefinition &bomberman::BombUpPickup::GetSprite() const {
    return GetTileset().GetPickupSprite(PickupType::BombUp);
}

bomberman::PickupType bomberman::BombUpPickup::GetType() const {
    return PickupType::BombUp;
}

const bomberman::SpriteDefinition &bomberman::FlameUpPickup::GetSprite() const {
    return GetTileset().GetPickupSprite(PickupType::FlameUp);
}

bomberman::PickupType bomberman::FlameUpPickup::GetType() const {
    return PickupType::FlameUp;
}

const bomberman::SpriteDefinition &bomberman::DetonatorPickup::GetSprite() const {
    return GetTileset().GetPickupSprite(PickupType::Detonator);
}

bomberman::PickupType bomberman::DetonatorPickup::GetType() const {
    return PickupType::Detonator;
}

std::unique_ptr<bomberman::Pickup> bomberman::MakePickup(PickupType type, glm::ivec2 cell) {
    switch (type) {
        case PickupType::BombUp:
            return std::make_unique<BombUpPickup>(cell);
        case PickupType::FlameUp:
            return std::make_unique<FlameUpPickup>(cell);
        case PickupType::Detonator:
            return std::make_unique<DetonatorPickup>(cell);
    }

    return nullptr;
}
