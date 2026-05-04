#pragma once
#include "../../Core/Components/Component.h"
#include "../../Core/Timer.h"
#include "../../Core/Patterns/MulticastDelegate.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace dae {
    class BombComponent;
    class LevelGridComponent;
    class Scene;

    struct ActiveExplosion {
        glm::ivec2 cell;
        Timer timer;
    };

    // the reason it is a component is that we get render and update calls automatically
    class BombManagerComponent final : public Component {
    public:
        BombManagerComponent(GameObject *parent, Scene &scene, LevelGridComponent *gridComponent);

        void Update(float deltaTime) override;
        void Render() const override;

        static constexpr int BLAST_RADIUS = 2;

        void PlaceBomb(glm::ivec2 cell, GameObject *owner);
        void DetonateBomb(BombComponent *bomb);
        void RegisterPlayer(GameObject *player);
        void RegisterDamageable(GameObject *entity);

        MulticastDelegate<glm::ivec2> OnExplosionAt;

        float m_fuseTime = 3.0f;
        int m_maxBombsPerPlayer = 1;

    private:
        void SpawnExplosionAt(glm::ivec2 cell);
        void SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range);
        void ProcessDetonationQueue();

        [[nodiscard]] BombComponent *BombAt(glm::ivec2 cell) const;
        [[nodiscard]] size_t BombIndex(glm::ivec2 cell) const;

        Scene &m_scene;
        LevelGridComponent *m_grid;
        std::vector<BombComponent *> m_bombAtCell;
        std::vector<BombComponent *> m_detonationQueue;
        std::unordered_map<GameObject *, int> m_playerBombCount;
        std::vector<GameObject *> m_damageables;
        std::vector<ActiveExplosion> m_activeExplosions;
    };
}
