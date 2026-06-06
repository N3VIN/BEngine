#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace bomberman {
    class LevelGridComponent;
    class GridMovementComponent;

    class ExitComponent final : public bengine::Component {
    public:
        ExitComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent);

        void Update(float deltaTime) override;

        void RegisterPlayer(const bengine::GameObject *player);

    private:
        void RevealExit();

        LevelGridComponent *m_grid{nullptr};
        glm::ivec2 m_exitCell{-1, -1};
        std::vector<GridMovementComponent *> m_players{};
        bool m_revealed{false};
        bool m_enemiesCleared{false};
        bool m_completed{false};
        bengine::ScopedDelegate m_brickDestroyedSub;
        bengine::ScopedDelegate m_enemiesDefeatedSub;
    };
}
