#pragma once
#include <filesystem>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Level/TileType.h"

namespace fs = std::filesystem;

namespace dae {
    class GameObject;

    class LevelGridComponent final : public Component {
    public:
        explicit LevelGridComponent(GameObject *parent, const fs::path &levelPath);
        ~LevelGridComponent() override = default;

        LevelGridComponent(const LevelGridComponent &) = delete;
        LevelGridComponent(LevelGridComponent &&) = delete;
        LevelGridComponent &operator=(const LevelGridComponent &) = delete;
        LevelGridComponent &operator=(LevelGridComponent &&) = delete;

        MulticastDelegate<glm::ivec2> OnBrickDestroyed;

        [[nodiscard]] bool IsWalkable(glm::ivec2 cell) const;
        [[nodiscard]] bool IsBrick(glm::ivec2 cell) const;
        [[nodiscard]] bool HasWall(glm::ivec2 cell) const;
        [[nodiscard]] bool InBounds(glm::ivec2 cell) const;
        [[nodiscard]] glm::vec2 CellToWorld(glm::ivec2 cell) const;

        [[nodiscard]] float GetCellSize() const {
            return m_cellSize;
        }

        [[nodiscard]] int GetColumns() const {
            return m_columns;
        }

        [[nodiscard]] int GetRows() const {
            return m_rows;
        }

        [[nodiscard]] glm::ivec2 GetDimensions() const {
            return {m_columns, m_rows};
        }

        [[nodiscard]] glm::vec2 GetOrigin() const {
            return m_origin;
        }

        [[nodiscard]] TileType GetTile(int column, int row) const;

        void SetWall(glm::ivec2 cell, bool isWall);
        GameObject *DestroyBrick(glm::ivec2 cell);
        void SetTile(GameObject *tile, glm::ivec2 cell);

    private:
        [[nodiscard]] size_t Index(glm::ivec2 cell) const;
        static TileType ParseTile(char c);

        int m_columns{};
        int m_rows{};
        float m_cellSize{};
        glm::vec2 m_origin{};
        std::vector<TileType> m_tiles{};
        std::vector<uint8_t> m_walls{};
        std::vector<GameObject *> m_tileAtCell{};
    };
}
