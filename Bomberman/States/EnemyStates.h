#pragma once
#include <glm/glm.hpp>
#include "IState.h"
#include "Level/Tileset.h"

namespace bomberman {
    class EnemyWalkState final : public IEnemyState {
    public:
        EnemyWalkState(bool facingLeft, const SpriteDefinition &clip);
        void OnEnter(EnemyStateComponent &state) override;
        std::unique_ptr<IEnemyState> Update(EnemyStateComponent &state, float deltaTime) override;

        [[nodiscard]] const SpriteDefinition *GetClip() const override {
            return &m_clip;
        }

    private:
        bool m_facingLeft;
        SpriteDefinition m_clip;
    };

    std::unique_ptr<IEnemyState> MakeEnemyWalkState(glm::ivec2 facing, const EnemySprites &sprites);
}
