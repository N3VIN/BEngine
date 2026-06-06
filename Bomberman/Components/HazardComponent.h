#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace bomberman {
    class LevelGridComponent;
    class GridMovementComponent;
    class HealthComponent;

    class HazardComponent final : public bengine::Component {
    public:
        HazardComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent);

        void Update(float deltaTime) override;

        void RegisterPlayer(bengine::GameObject *player);
        void RegisterEnemy(bengine::GameObject *enemy);

    private:
        struct Damageable {
            bengine::GameObject *object{nullptr};
            GridMovementComponent *movement{nullptr};
            HealthComponent *health{nullptr};
        };

        [[nodiscard]] bool ExplosionActive(glm::ivec2 cell) const;
        [[nodiscard]] size_t Index(glm::ivec2 cell) const;

        LevelGridComponent *m_grid{nullptr};
        float m_explosionLifetime{};
        std::vector<float> m_hazardTime{}; // easier than a list of cells like m_bombAtCell
        std::vector<Damageable> m_players{};
        std::vector<Damageable> m_enemies{};
        bengine::ScopedDelegate m_explosionSub;
        bengine::ScopedDelegate m_enemyKilledSub;
    };
}
