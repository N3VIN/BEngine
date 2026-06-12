#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "States/IPlayerState.h"
#include "Level/Tileset.h"

namespace bomberman {
    class GridMovementComponent;
    class SpriteRendererComponent;
    class HealthComponent;

    class PlayerControllerComponent final : public bengine::Component {
    public:
        PlayerControllerComponent(bengine::GameObject *parent, PlayerSprites sprites);
        void Update(float deltaTime) override;

        [[nodiscard]] const PlayerSprites &GetSprites() const;
        [[nodiscard]] glm::ivec2 GetFacing() const;
        [[nodiscard]] bool IsMoving() const;
        [[nodiscard]] bool IsAnimationPlaying() const;
        [[nodiscard]] int GetLives() const;

        void PlayAnimation(const SpriteDefinition &clip, bool loop) const;
        void StopMovement() const;
        void Respawn() const;
        void NotifyDied() const;

    private:
        void PlayStep(glm::ivec2 facing) const;

        GridMovementComponent *m_movement{};
        SpriteRendererComponent *m_sprite{};
        HealthComponent *m_health{};
        PlayerSprites m_sprites{};
        const glm::ivec2 m_spawnCell{};

        std::unique_ptr<IPlayerState> m_currentState;
        bool m_damaged{false};
        glm::ivec2 m_lastStepCell{};
        bengine::ScopedDelegate m_damagedSub;
    };
}
