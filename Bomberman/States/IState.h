#pragma once
#include <memory>
#include <type_traits>

namespace bomberman {
    struct SpriteDefinition;
    class PlayerStateComponent;
    class EnemyStateComponent;

    template<typename StateComponent>
    concept State = std::is_class_v<StateComponent>;

    template<State StateComponent>
    class IState {
    public:
        virtual ~IState() = default;

        virtual void OnEnter(StateComponent & /*parent*/) {}
        virtual void OnExit(StateComponent & /*parent*/) {}

        virtual std::unique_ptr<IState> Update(StateComponent & /*parent*/, float /*deltaTime*/) {
            return nullptr;
        }

        [[nodiscard]] virtual bool IsAlive() const {
            return true;
        }

        [[nodiscard]] virtual const SpriteDefinition *GetClip() const {
            return nullptr;
        }

    protected:
        IState() = default;
    };

    using IPlayerState = IState<PlayerStateComponent>;
    using IEnemyState = IState<EnemyStateComponent>;
}
