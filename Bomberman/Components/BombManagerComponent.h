#pragma once
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "SceneGraph/Scene.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace bomberman {
    class BombComponent;
    class LevelGridComponent;

    class BombManagerComponent final : public bengine::Component {
    public:
        BombManagerComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent);

        void PlaceBomb(glm::ivec2 cell, bengine::GameObject *owner);
        void DetonateBomb(BombComponent *bomb);
        void DetonateOldestBomb(bengine::GameObject *owner);
        void RegisterPlayer(bengine::GameObject *player);

        int m_maxBombsPerPlayer = 2;
        int m_blastRadius = 2;

    private:
        void SpawnExplosionAt(glm::ivec2 cell);
        void SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range);
        void ProcessDetonationQueue();

        [[nodiscard]] bengine::GameObject *BombAt(glm::ivec2 cell) const;
        [[nodiscard]] size_t BombIndex(glm::ivec2 cell) const;

        LevelGridComponent *m_gridComponent;
        std::vector<bengine::GameObject *> m_bombAtCell;
        std::vector<BombComponent *> m_activeBombs;
        std::vector<BombComponent *> m_detonationQueue;
        std::unordered_map<bengine::GameObject *, int> m_playerBombCount;
    };
}
