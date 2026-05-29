#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "States/IPlayerState.h"

namespace bomberman {
    class GridMovementComponent;
    class SpriteRendererComponent;
    class HealthComponent;

    class PlayerStateComponent final : public bengine::Component {
    public:
        explicit PlayerStateComponent(bengine::GameObject *parent);
        void Update(float deltaTime) override;

        [[nodiscard]] GridMovementComponent *GetMovement() const {
            return m_movement;
        }

        [[nodiscard]] SpriteRendererComponent *GetSprite() const {
            return m_sprite;
        }

        [[nodiscard]] HealthComponent *GetHealth() const {
            return m_health;
        }

        [[nodiscard]] glm::ivec2 GetSpawnCell() const {
            return m_spawnCell;
        }

        [[nodiscard]] bengine::GameObject *GetPlayerObject() const {
            return GetParent();
        }

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
