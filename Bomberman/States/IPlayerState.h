#pragma once
#include <memory>

namespace bomberman {
    class PlayerStateComponent;

    class IPlayerState {
    public:
        virtual ~IPlayerState() = default;

        virtual void OnEnter(PlayerStateComponent & /*state*/) {}
        virtual void OnExit(PlayerStateComponent & /*state*/) {}

        virtual std::unique_ptr<IPlayerState> Update(PlayerStateComponent & /*state*/, float /*deltaTime*/) {
            return nullptr;
        }

        [[nodiscard]] virtual bool IsAlive() const {
            return true;
        }

    protected:
        IPlayerState() = default;
    };
}
