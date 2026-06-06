#pragma once
#include <glm/glm.hpp>
#include "IPlayerState.h"
#include "Level/Tileset.h"

namespace bomberman {
    class IdlePlayerState final : public IPlayerState {
    public:
        void OnEnter(PlayerControllerComponent &controller) override;
        std::unique_ptr<IPlayerState> Update(PlayerControllerComponent &controller, float deltaTime) override;
    };

    class DirectionalWalkState final : public IPlayerState {
    public:
        DirectionalWalkState(glm::ivec2 direction, const SpriteDefinition &clip);
        void OnEnter(PlayerControllerComponent &controller) override;
        std::unique_ptr<IPlayerState> Update(PlayerControllerComponent &controller, float deltaTime) override;

    private:
        glm::ivec2 m_direction;
        SpriteDefinition m_clip;
    };

    class DyingPlayerState final : public IPlayerState {
    public:
        void OnEnter(PlayerControllerComponent &controller) override;
        std::unique_ptr<IPlayerState> Update(PlayerControllerComponent &controller, float deltaTime) override;

        [[nodiscard]] bool IsAlive() const override;
    };

    class DeadPlayerState final : public IPlayerState {
    public:
        [[nodiscard]] bool IsAlive() const override;
    };

    SpriteDefinition GetWalkClip(glm::ivec2 facing);
    std::unique_ptr<IPlayerState> MakeWalkState(glm::ivec2 facing);
}
