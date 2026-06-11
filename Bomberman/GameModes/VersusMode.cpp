#include "GameModes/VersusMode.h"

#include "PlayerControls.h"
#include "SceneStates/GameEndState.h"
#include "Components/CameraComponent.h"
#include "Components/GridMovementComponent.h"
#include "SceneGraph/GameObject.h"

#include <string>

int bomberman::VersusMode::PlayerCount() const {
    return 2;
}

bool bomberman::VersusMode::SpawnsEnemies() const {
    return false;
}

bool bomberman::VersusMode::ShowsScore() const {
    return false;
}

void bomberman::VersusMode::ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const {
    camera.SetTargets({players[0], players[1]});

    for (const auto *player: players) {
        if (auto *movement = player->GetComponent<GridMovementComponent>()) {
            movement->SetViewClamp(&camera);
        }
    }
}

void bomberman::VersusMode::ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const {
    BindKeyboardControls(input, players[0], bombs);
    BindGamepadControls(input, 0, players[1], bombs);
}

std::unique_ptr<bengine::ISceneState> bomberman::VersusMode::MakeGameOverState(const bengine::GameObject *deadPlayer, const std::vector<bengine::GameObject *> &players) const {
    const int winner = (deadPlayer == players[0]) ? 2 : 1;
    return std::make_unique<GameEndState>("PLAYER " + std::to_string(winner) + " WINS");
}
