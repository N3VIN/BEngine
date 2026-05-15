#pragma once
#include "Components/Component.h"
#include "Timer.h"
#include <glm/glm.hpp>

namespace dae {
    class ExplosionComponent final : public Component {
    public:
        ExplosionComponent(GameObject *parent, glm::ivec2 cell, float lifetime);
        ~ExplosionComponent() override = default;

        void Update(float deltaTime) override;
        void ExtendLifetime(float lifetime);

        [[nodiscard]] glm::ivec2 GetCell() const {
            return m_cell;
        }

    private:
        glm::ivec2 m_cell;
        Timer m_timer;
    };
}
