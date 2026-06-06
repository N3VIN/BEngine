#pragma once
#include <memory>

namespace bomberman {
    class PlayerControllerComponent;

    class IPlayerState {
    public:
        virtual ~IPlayerState() = default;

        virtual void OnEnter(PlayerControllerComponent & /*controller*/) {}
        virtual void OnExit(PlayerControllerComponent & /*controller*/) {}

        virtual std::unique_ptr<IPlayerState> Update(PlayerControllerComponent & /*controller*/, float /*deltaTime*/) {
            return nullptr;
        }

        [[nodiscard]] virtual bool IsAlive() const {
            return true;
        }

    protected:
        IPlayerState() = default;
    };
}
