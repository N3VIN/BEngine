#pragma once
#include <filesystem>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "SceneGraph/GameObject.h"
#include "Level/TileType.h"
#include "Types/PickupType.h"

namespace fs = std::filesystem;

namespace bomberman {
    struct PickupSpawn {
        glm::ivec2 cell{};
        PickupType type{};
    };

    class LevelGridComponent final : public bengine::Component {
    public:
        explicit LevelGridComponent(bengine::GameObject *parent, const fs::path &levelPath);
        ~LevelGridComponent() override = default;

        LevelGridComponent(const LevelGridComponent &) = delete;
        LevelGridComponent(LevelGridComponent &&) = delete;
        LevelGridComponent &operator=(const LevelGridComponent &) = delete;
        LevelGridComponent &operator=(LevelGridComponent &&) = delete;

        [[nodiscard]] bool IsWalkable(glm::ivec2 cell) const;
        [[nodiscard]] bool HasClearPath(glm::ivec2 origin, glm::ivec2 target) const;
        [[nodiscard]] bool IsBrick(glm::ivec2 cell) const;
        [[nodiscard]] bool HasWall(glm::ivec2 cell) const;
        [[nodiscard]] bool InBounds(glm::ivec2 cell) const;
        [[nodiscard]] glm::vec2 CellToWorld(glm::ivec2 cell) const;
        [[nodiscard]] float GetCellSize() const;
        [[nodiscard]] int GetColumns() const;
        [[nodiscard]] int GetRows() const;
        [[nodiscard]] glm::ivec2 GetDimensions() const;
        [[nodiscard]] glm::vec2 GetOrigin() const;
        [[nodiscard]] TileType GetTile(int column, int row) const;
        [[nodiscard]] glm::ivec2 GetExitCell() const;
        [[nodiscard]] const std::vector<PickupSpawn> &GetPickups() const;

        void SetWall(glm::ivec2 cell, bool isWall);
        bengine::GameObject *DestroyBrick(glm::ivec2 cell);
        void SetTile(bengine::GameObject *tile, glm::ivec2 cell);

    private:
        [[nodiscard]] size_t Index(glm::ivec2 cell) const;
        static TileType ParseTile(char c);

        int m_columns{};
        int m_rows{};
        float m_cellSize{};
        glm::vec2 m_origin{};
        glm::ivec2 m_exitCell{-1, -1};
        std::vector<PickupSpawn> m_pickups{};
        std::vector<TileType> m_tiles{};
        std::vector<uint8_t> m_walls{};
        std::vector<bengine::GameObject *> m_tileAtCell{};
    };
}
