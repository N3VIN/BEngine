#pragma once
#include "GameModes/IGameMode.h"

namespace bomberman {
    class CoopMode final : public IGameMode {
    public:
        [[nodiscard]] int PlayerCount() const override;
        [[nodiscard]] bool SpawnsEnemies() const override;

        void ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const override;
        void ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const override;

        [[nodiscard]] std::unique_ptr<bengine::ISceneState> MakeGameOverState(const bengine::GameObject *deadPlayer, const std::vector<bengine::GameObject *> &players) const override;
    };
}
