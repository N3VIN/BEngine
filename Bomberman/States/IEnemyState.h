#pragma once
#include <memory>

namespace bomberman {
    class EnemyControllerComponent;

    class IEnemyState {
    public:
        virtual ~IEnemyState() = default;

        virtual void OnEnter(EnemyControllerComponent & /*controller*/) {}
        virtual void OnExit(EnemyControllerComponent & /*controller*/) {}

        virtual std::unique_ptr<IEnemyState> Update(EnemyControllerComponent & /*controller*/, float /*deltaTime*/) {
            return nullptr;
        }

        [[nodiscard]] virtual bool IsAlive() const {
            return true;
        }

    protected:
        IEnemyState() = default;
    };
}
