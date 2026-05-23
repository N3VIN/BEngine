#pragma once
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include "Patterns/MulticastDelegate.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace bomberman {
    class BombComponent;
    class LevelGridComponent;

    class BombManagerComponent final : public bengine::Component {
    public:
        BombManagerComponent(bengine::GameObject *parent, bengine::Scene *scene, LevelGridComponent *gridComponent);

        static constexpr int BLAST_RADIUS = 2;

        void PlaceBomb(glm::ivec2 cell, bengine::GameObject *owner);
        void DetonateBomb(BombComponent *bomb);
        void RegisterPlayer(bengine::GameObject *player);

        float m_fuseTime = 3.0f;
        int m_maxBombsPerPlayer = 2;

    private:
        void SpawnExplosionAt(glm::ivec2 cell);
        void SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range);
        void ProcessDetonationQueue();
        void OnExplosionCellExpired(glm::ivec2 cell);

        [[nodiscard]] bengine::GameObject *BombAt(glm::ivec2 cell) const;
        [[nodiscard]] size_t BombIndex(glm::ivec2 cell) const;

        bengine::Scene *m_scene;
        LevelGridComponent *m_gridComponent;
        std::vector<bengine::GameObject *> m_bombAtCell;
        std::vector<bengine::GameObject *> m_bombOwnerAtCell;
        std::vector<BombComponent *> m_detonationQueue;
        std::unordered_map<bengine::GameObject *, int> m_playerBombCount;
        std::vector<bengine::GameObject *> m_explosionAtCell;
        bengine::ScopedDelegate m_detonationSub;
        bengine::ScopedDelegate m_explosionExpiredSub;
    };
}
