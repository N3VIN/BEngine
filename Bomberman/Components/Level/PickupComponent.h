#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Gameplay/Pickups.h"

namespace bomberman {
    class LevelGridComponent;

    class PickupComponent final : public bengine::Component {
    public:
        PickupComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent);

        void Update(float deltaTime) override;
        void RegisterPlayer(bengine::GameObject *player);

    private:
        void Reveal(Pickup &pickup) const;
        [[nodiscard]] bengine::GameObject *PlayerAt(glm::ivec2 cell) const;

        LevelGridComponent *m_grid{nullptr};
        std::vector<std::unique_ptr<Pickup> > m_pickups{};
        std::vector<bengine::GameObject *> m_players{};
        bengine::ScopedDelegate m_brickDestroyedSub;
    };
}
