#pragma once
#include <memory>

namespace bomberman {
    class PlayerStateComponent;
    struct SpriteDefinition;

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

        [[nodiscard]] virtual const SpriteDefinition *GetClip() const {
            return nullptr;
        }

    protected:
        IPlayerState() = default;
    };
}
