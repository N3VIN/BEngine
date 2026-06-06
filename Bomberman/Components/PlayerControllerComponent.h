#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "States/IPlayerState.h"

namespace bomberman {
    struct SpriteDefinition;
    class GridMovementComponent;
    class SpriteRendererComponent;
    class HealthComponent;

    class PlayerControllerComponent final : public bengine::Component {
    public:
        explicit PlayerControllerComponent(bengine::GameObject *parent);
        void Update(float deltaTime) override;

        [[nodiscard]] glm::ivec2 GetFacing() const;
        [[nodiscard]] bool IsMoving() const;
        [[nodiscard]] bool IsAnimationPlaying() const;
        [[nodiscard]] int GetLives() const;

        void PlayAnimation(const SpriteDefinition &clip, bool loop) const;
        void Respawn() const;
        void NotifyDied() const;

    private:
        GridMovementComponent *m_movement{};
        SpriteRendererComponent *m_sprite{};
        HealthComponent *m_health{};
        const glm::ivec2 m_spawnCell{};

        std::unique_ptr<IPlayerState> m_currentState;
        bool m_damaged{false};
        bengine::ScopedDelegate m_damagedSub;
    };
}
