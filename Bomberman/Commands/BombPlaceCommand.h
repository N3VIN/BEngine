#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/GridMovementComponent.h"
#include "Components/BombManagerComponent.h"

namespace bomberman {
    class BombPlaceCommand final : public bengine::GameObjectCommand {
    public:
        BombPlaceCommand(bengine::GameObject *player, BombManagerComponent *bombManager)
            : bengine::GameObjectCommand(player)
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
