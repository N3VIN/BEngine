#include "GameModes/SoloMode.h"

#include "PlayerControls.h"
#include "SceneStates/GameEndState.h"
#include "Components/CameraComponent.h"

int bomberman::SoloMode::PlayerCount() const {
    return 1;
}

bool bomberman::SoloMode::SpawnsEnemies() const {
    return true;
}

void bomberman::SoloMode::ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const {
    camera.SetTarget(players[0]);
}

void bomberman::SoloMode::ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const {
    BindKeyboardControls(input, players[0], bombs);
    BindGamepadControls(input, 0, players[0], bombs);
}

std::unique_ptr<bengine::ISceneState> bomberman::SoloMode::MakeGameOverState(const bengine::GameObject *, const std::vector<bengine::GameObject *> &) const {
    return std::make_unique<GameEndState>();
}
