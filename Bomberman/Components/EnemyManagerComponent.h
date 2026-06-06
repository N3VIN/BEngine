#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "EnemyType.h"

namespace bengine {
    class Scene;
}

namespace bomberman {
    class LevelGridComponent;
    class HazardComponent;

    class EnemyManagerComponent final : public bengine::Component {
    public:
        EnemyManagerComponent(bengine::GameObject *parent, bengine::Scene *scene, LevelGridComponent *gridComponent, HazardComponent *hazardComponent);

        void RegisterPlayer(bengine::GameObject *player);
        void SpawnEnemy(EnemyType type, glm::ivec2 cell);

    private:
        bengine::Scene *m_scene{};
        LevelGridComponent *m_gridComponent{};
        HazardComponent *m_hazardComponent{};
        std::vector<bengine::GameObject *> m_enemies{};
        std::vector<bengine::GameObject *> m_players{};
        bengine::ScopedDelegate m_killedSub;
    };
}
