#pragma once
#include <glm/glm.hpp>
#include "IEnemyState.h"

namespace bomberman {
    class EnemyWalkState final : public IEnemyState {
    public:
        explicit EnemyWalkState(bool facingLeft);
        void OnEnter(EnemyControllerComponent &controller) override;
        std::unique_ptr<IEnemyState> Update(EnemyControllerComponent &controller, float deltaTime) override;

    private:
        bool m_facingLeft;
    };

    class EnemyDyingState final : public IEnemyState {
    public:
        void OnEnter(EnemyControllerComponent &controller) override;
        std::unique_ptr<IEnemyState> Update(EnemyControllerComponent &controller, float deltaTime) override;

        [[nodiscard]] bool IsAlive() const override;
    };

    std::unique_ptr<IEnemyState> MakeEnemyWalkState(glm::ivec2 facing);
}
