#pragma once
#include <memory>
#include <vector>

namespace bengine {
    class GameObject;
    class CameraComponent;
    class InputManager;
    class ISceneState;
}

namespace bomberman {
    class BombManagerComponent;

    class IGameMode {
    public:
        virtual ~IGameMode() = default;

        [[nodiscard]] virtual int PlayerCount() const = 0;
        [[nodiscard]] virtual bool SpawnsEnemies() const = 0;
        [[nodiscard]] virtual bool ShowsScore() const = 0;

        virtual void ConfigureCamera(bengine::CameraComponent &camera, const std::vector<bengine::GameObject *> &players) const = 0;
        virtual void ConfigureInput(bengine::InputManager &input, const std::vector<bengine::GameObject *> &players, BombManagerComponent &bombs) const = 0;

        [[nodiscard]] virtual std::unique_ptr<bengine::ISceneState> MakeGameOverState(const bengine::GameObject *deadPlayer, const std::vector<bengine::GameObject *> &players) const = 0;

    protected:
        IGameMode() = default;
    };
}
