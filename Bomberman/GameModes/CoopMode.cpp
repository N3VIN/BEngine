#include "GameModes/CoopMode.h"

#include "PlayerControls.h"
#include "SceneStates/GameEndState.h"
#include "Components/CameraComponent.h"

int bomberman::CoopMode::PlayerCount() const {
    return 2;
}

bool bomberman::CoopMode::SpawnsEnemies() const {
    return true;
}

void bomberman::CoopMode::ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const {
    camera.SetTarget(players[0]);
}

void bomberman::CoopMode::ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const {
    BindKeyboardControls(input, players[0], bombs);
    BindGamepadControls(input, 0, players[1], bombs);
}

std::unique_ptr<bengine::ISceneState> bomberman::CoopMode::MakeGameOverState(const bengine::GameObject *, const std::vector<bengine::GameObject *> &) const {
    return std::make_unique<GameEndState>();
}
