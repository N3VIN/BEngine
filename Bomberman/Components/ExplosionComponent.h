#pragma once
#include "Components/Component.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace bomberman {
    class ExplosionComponent final : public bengine::Component {
    public:
        ExplosionComponent(bengine::GameObject *parent, glm::ivec2 cell, float lifetime);
        ~ExplosionComponent() override = default;

        void Update(float deltaTime) override;
        void ExtendLifetime(float lifetime);

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

    private:
        glm::ivec2 m_cell;
        bengine::Timer m_timer;
    };
}
