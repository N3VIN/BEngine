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
    struct SpriteDefinition;

    class BombManagerComponent final : public bengine::Component {
    public:
        BombManagerComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent);

        void Update(float deltaTime) override;

        void PlaceBomb(glm::ivec2 cell, bengine::GameObject *owner);
        void DetonateBomb(BombComponent *bomb);
        void DetonateOldestBomb(bengine::GameObject *owner);
        void RegisterPlayer(bengine::GameObject *player);

    private:
        void AddBomb(bengine::GameObject *owner);
        void AddFlame(bengine::GameObject *owner);
        void GiveDetonator(bengine::GameObject *owner);

        struct PlayerBombStats {
            int activeBombs{0};
            int maxBombs{1};
            int blastRadius{1};
            bool hasDetonator{false};
        };

        static constexpr float BOMB_FUSE_DURATION{2.5f};
        static constexpr int MAX_BOMBS{9};
        static constexpr int MAX_BLAST_RADIUS{4};

        void SpawnFlame(glm::ivec2 cell, const SpriteDefinition &piece) const;
        void SpreadInDirection(glm::ivec2 origin, glm::ivec2 direction, int range, bengine::GameObject *owner);
        void ProcessDetonationQueue();

        [[nodiscard]] bengine::GameObject *BombAt(glm::ivec2 cell) const;
        [[nodiscard]] bool IsFlameBlocker(glm::ivec2 cell) const;
        [[nodiscard]] size_t BombIndex(glm::ivec2 cell) const;

        LevelGridComponent *m_gridComponent;
        std::vector<bengine::GameObject *> m_bombAtCell;
        std::vector<BombComponent *> m_activeBombs;
        std::vector<BombComponent *> m_detonationQueue;
        std::unordered_map<bengine::GameObject *, PlayerBombStats> m_playerStats;
        std::vector<bengine::ScopedDelegate> m_damagedSubs;
        bengine::ScopedDelegate m_pickupSub;
    };
}
