#pragma once
#include <glm/glm.hpp>
#include "IState.h"
#include "Level/Tileset.h"

namespace bomberman {
    class IdlePlayerState final : public IPlayerState {
    public:
        void OnEnter(PlayerStateComponent &state) override;
        std::unique_ptr<IPlayerState> Update(PlayerStateComponent &state, float deltaTime) override;
    };

    class DirectionalWalkState : public IPlayerState {
    public:
        DirectionalWalkState(glm::ivec2 direction, const SpriteDefinition &clip);
        void OnEnter(PlayerStateComponent &state) override;
        std::unique_ptr<IPlayerState> Update(PlayerStateComponent &state, float deltaTime) override;

        [[nodiscard]] const SpriteDefinition *GetClip() const override {
            return &m_clip;
        }

    private:
        glm::ivec2 m_direction;
        SpriteDefinition m_clip;
    };

    class WalkUpState final : public DirectionalWalkState {
    public:
        WalkUpState();
    };

    class WalkDownState final : public DirectionalWalkState {
    public:
        WalkDownState();
    };

    class WalkLeftState final : public DirectionalWalkState {
    public:
        WalkLeftState();
    };

    class WalkRightState final : public DirectionalWalkState {
    public:
        WalkRightState();
    };

    class DyingPlayerState final : public IPlayerState {
    public:
        void OnEnter(PlayerStateComponent &state) override;
        std::unique_ptr<IPlayerState> Update(PlayerStateComponent &state, float deltaTime) override;

        [[nodiscard]] bool IsAlive() const override {
            return false;
        }
    };

    class DeadPlayerState final : public IPlayerState {
    public:
        [[nodiscard]] bool IsAlive() const override {
            return false;
        }
    };

    std::unique_ptr<IPlayerState> MakeWalkState(glm::ivec2 facing);
}
