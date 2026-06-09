#pragma once
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace bomberman {
    class BombComponent final : public bengine::Component {
    public:
        BombComponent(bengine::GameObject *parent, glm::ivec2 cell, int blastRadius, bengine::GameObject *owner, float fuseDuration, bool remote);

        void MarkDetonated();
        void TickFuse(float deltaTime);

        [[nodiscard]] bool IsDetonated() const;
        [[nodiscard]] bool FuseExpired() const;
        [[nodiscard]] glm::ivec2 GetCell() const;
        [[nodiscard]] int GetRadius() const;
        [[nodiscard]] bengine::GameObject *GetOwner() const;
        [[nodiscard]] bengine::GameObject *GetGameObject() const;

    private:
        glm::ivec2 m_cell;
        int m_blastRadius;
        bengine::GameObject *m_owner;
        bengine::Timer m_fuse;
        bool m_remote;
        bool m_detonated{false};
    };
}
