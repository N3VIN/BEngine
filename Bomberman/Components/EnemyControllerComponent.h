#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "EnemyType.h"
#include "States/IEnemyState.h"

namespace bengine {
    class Scene;
}

namespace bomberman {
    class GridMovementComponent;
    class SpriteRendererComponent;
    struct EnemySprites;

    class EnemyControllerComponent final : public bengine::Component {
    public:
        EnemyControllerComponent(bengine::GameObject *parent, bengine::Scene *scene, EnemyType type);
        void Update(float deltaTime) override;

        [[nodiscard]] bool IsAlive() const;
        [[nodiscard]] glm::ivec2 GetFacing() const;
        [[nodiscard]] bool IsAnimationPlaying() const;

        void PlayWalkAnimation(bool facingLeft) const;
        void PlayDeathAnimation() const;
        void Die();

    private:
        bengine::Scene *m_scene{};
        GridMovementComponent *m_movement{};
        SpriteRendererComponent *m_sprite{};
        const EnemySprites *m_sprites{};
        int m_points{};
        std::unique_ptr<IEnemyState> m_currentState;
        bool m_killed{false};
        bengine::ScopedDelegate m_explosionSub;
    };
}
