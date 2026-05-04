#pragma once
#include "../../Core/Patterns/ICommand.h"
#include "../../Core/SceneGraph/GameObject.h"
#include "../Components/GridMovementComponent.h"
#include "../Components/BombManagerComponent.h"

namespace dae {
    class BombPlaceCommand final : public GameObjectCommand {
    public:
        BombPlaceCommand(GameObject *player, BombManagerComponent *bombManager)
            : GameObjectCommand(player)
          , m_bombManager(bombManager) {}

        void Execute() override {
            auto *player = GetGameObject();
            auto *movement = player->GetComponent<GridMovementComponent>();
            if (!movement) {
                return;
            }

            m_bombManager->PlaceBomb(movement->GetCell(), player);
        }

    private:
        BombManagerComponent *m_bombManager;
    };
}
