#include "GameModes/CoopMode.h"

#include "PlayerControls.h"
#include "SceneStates/GameEndState.h"
#include "Components/CameraComponent.h"
#include "Components/GridMovementComponent.h"
#include "SceneGraph/GameObject.h"

int bomberman::CoopMode::PlayerCount() const {
    return 2;
}

bool bomberman::CoopMode::SpawnsEnemies() const {
    return true;
}

bool bomberman::CoopMode::ShowsScore() const {
    return true;
}

bool bomberman::CoopMode::Player2IsBalloom() const {
    return false;
}

void bomberman::CoopMode::ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const {
    camera.SetTargets({players[0], players[1]});

    for (const auto *player: players) {
        if (auto *movement = player->GetComponent<GridMovementComponent>()) {
            movement->SetViewClamp(&camera);
        }
    }
}

void bomberman::CoopMode::ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const {
    BindKeyboardControls(input, players[0], bombs);
    BindGamepadControls(input, 0, players[1], bombs);
}

std::unique_ptr<bengine::ISceneState> bomberman::CoopMode::MakeGameOverState(const bengine::GameObject *, const std::vector<bengine::GameObject *> &) const {
    return std::make_unique<GameEndState>();
}
