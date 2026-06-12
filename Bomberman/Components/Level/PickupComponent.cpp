#include "PickupComponent.h"
#include "LevelGridComponent.h"
#include "Components/GridMovementComponent.h"
#include "Components/SpriteSetup.h"
#include "Level/Tileset.h"
#include "Gameplay/GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"

bomberman::PickupComponent::PickupComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent)
    : bengine::Component(parent)
  , m_grid(gridComponent) {
    for (const auto &[cell, type]: m_grid->GetPickups()) {
        m_pickups.push_back(MakePickup(type, cell));
    }

    m_brickDestroyedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::BrickDestroyed>(
        [this](const events::BrickDestroyed &event) {
            for (auto &pickup: m_pickups) {
                if (!pickup->IsRevealed() && pickup->GetCell() == event.cell) {
                    Reveal(*pickup);
                }
            }
        }
    );
}

void bomberman::PickupComponent::Update(float) {
    std::erase_if(
        m_pickups, [this](const std::unique_ptr<Pickup> &pickup) {
            if (!pickup->IsRevealed()) {
                return false;
            }

            auto *player = PlayerAt(pickup->GetCell());
            if (!player) {
                return false;
            }

            bengine::GetActiveScene()->Remove(pickup->GetObject());
            bengine::ServiceLocator::GetEventBus().Broadcast(events::PickupCollected{
                    .player = player,
                    .type = pickup->GetType()
                }
            );

            return true;
        }
    );
}

void bomberman::PickupComponent::RegisterPlayer(bengine::GameObject *player) {
    m_players.push_back(player);
}

void bomberman::PickupComponent::Reveal(Pickup &pickup) const {
    auto pickupGO = std::make_unique<bengine::GameObject>();
    pickupGO->SetLocalPosition(m_grid->CellToWorld(pickup.GetCell()));

    auto *sprite = AddSprite(*pickupGO);
    sprite->Play(pickup.GetSprite(), false, GetTileset().animationFps);

    pickup.MarkRevealed(bengine::GetActiveScene()->Add(std::move(pickupGO)));
}

bengine::GameObject *bomberman::PickupComponent::PlayerAt(glm::ivec2 cell) const {
    const auto playerGO = std::ranges::find_if(
        m_players, [cell](const bengine::GameObject *player) {
            return player->GetComponent<GridMovementComponent>()->GetCell() == cell; // if player is on the cell
        }
    );

    return playerGO != m_players.end() ? *playerGO : nullptr; // check if player was found
}
