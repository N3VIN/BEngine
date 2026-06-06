#pragma once
#include <memory>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "EnemyType.h"
#include "States/IState.h"

namespace bengine {
    class Scene;
}

namespace bomberman {
    class GridMovementComponent;
    class SpriteRendererComponent;
    struct EnemySprites;

    class EnemyStateComponent final : public bengine::Component {
    public:
        EnemyStateComponent(bengine::GameObject *parent, bengine::Scene *scene, EnemyType type);
        void Update(float deltaTime) override;

        void Die();

        [[nodiscard]] bool IsAlive() const {
            return m_currentState->IsAlive();
        }

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
        bengine::Scene *m_scene;
        GridMovementComponent *m_movement;
        SpriteRendererComponent *m_sprite;
        const EnemySprites *m_sprites;
        int m_points;
        std::unique_ptr<IEnemyState> m_currentState;
        bool m_killed{false};
        bengine::ScopedDelegate m_explosionSub;
    };
}
