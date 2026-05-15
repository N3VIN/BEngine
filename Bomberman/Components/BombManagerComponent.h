#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace dae {
    class BombComponent;
    class LevelGridComponent;
    class Scene;
    class GameObject;

    class BombManagerComponent final : public Component {
    public:
        BombManagerComponent(GameObject *parent, Scene *scene, LevelGridComponent *gridComponent);

        static constexpr int BLAST_RADIUS = 2;

        void PlaceBomb(glm::ivec2 cell, GameObject *owner);
        void DetonateBomb(BombComponent *bomb);
        void RegisterPlayer(GameObject *player);

        float m_fuseTime = 3.0f;
        int m_maxBombsPerPlayer = 2;

    private:
        void SpawnExplosionAt(glm::ivec2 cell);
        void SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range);
        void ProcessDetonationQueue();
        void OnExplosionCellExpired(glm::ivec2 cell);

        [[nodiscard]] GameObject *BombAt(glm::ivec2 cell) const;
        [[nodiscard]] size_t BombIndex(glm::ivec2 cell) const;

        Scene *m_scene;
        LevelGridComponent *m_gridComponent;
        std::vector<GameObject *> m_bombAtCell;
        std::vector<GameObject *> m_bombOwnerAtCell;
        std::vector<BombComponent *> m_detonationQueue;
        std::unordered_map<GameObject *, int> m_playerBombCount;
        std::vector<GameObject *> m_explosionAtCell;
        ScopedDelegate m_detonationSub;
        ScopedDelegate m_explosionExpiredSub;
    };
}
