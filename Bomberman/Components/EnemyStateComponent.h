#pragma once
#include <memory>
#include "Components/Component.h"
#include "EnemyType.h"
#include "States/IState.h"

namespace bomberman {
    class GridMovementComponent;
    class SpriteRendererComponent;
    struct EnemySprites;

    class EnemyStateComponent final : public bengine::Component {
    public:
        EnemyStateComponent(bengine::GameObject *parent, EnemyType type);
        void Update(float deltaTime) override;

        [[nodiscard]] GridMovementComponent *GetMovement() const {
            return m_movement;
        }

        [[nodiscard]] SpriteRendererComponent *GetSprite() const {
            return m_sprite;
        }

        [[nodiscard]] const EnemySprites &GetSprites() const {
            return *m_sprites;
        }

    private:
        GridMovementComponent *m_movement;
        SpriteRendererComponent *m_sprite;
        const EnemySprites *m_sprites;
        std::unique_ptr<IEnemyState> m_currentState;
    };
}
