#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/Bomb/BombManagerComponent.h"

namespace bomberman {
    class DetonateCommand final : public bengine::GameObjectCommand {
    public:
        DetonateCommand(bengine::GameObject *player, BombManagerComponent *bombManager)
            : bengine::GameObjectCommand(player)
          , m_bombManager(bombManager) {}

        void Execute() override {
            m_bombManager->DetonateOldestBomb(GetGameObject());
        }

    private:
        BombManagerComponent *m_bombManager;
    };
}
