#include "LevelGridComponent.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace dae {
    TileType LevelGridComponent::ParseTile(char tile) {
        switch (tile) {
            case '#':
                return TileType::Wall;
            case 'B':
                return TileType::Brick;
            case 'E':
                return TileType::Exit;
            case '1':
                return TileType::Player1Spawn;
            case '2':
                return TileType::Player2Spawn;
            default:
                return TileType::Empty;
        }
    }

    LevelGridComponent::LevelGridComponent(GameObject *parent, const fs::path &levelPath)
        : Component(parent) {
        if (!fs::exists(levelPath)) {
            throw std::runtime_error("Level file not found: " + levelPath.string());
        }

        std::ifstream file(levelPath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open level file: " + levelPath.string());
        }

        nlohmann::json json;
        file >> json;

        const auto &grid = json.at("grid");
        m_columns = grid.at("columns").get<int>();
        m_rows = grid.at("rows").get<int>();
        m_cellSize = grid.at("cellSize").get<float>();

        const auto &origin = json.at("origin");
        m_origin.x = origin.at("x").get<float>();
        m_origin.y = origin.at("y").get<float>();

        const auto &tiles = json.at("tiles");
        const size_t expectedSize = static_cast<size_t>(m_columns) * static_cast<size_t>(m_rows);
        m_tiles.reserve(expectedSize);

        for (const auto &row: tiles) {
            for (const std::string rowString = row.get<std::string>(); const char c: rowString) {
                m_tiles.push_back(ParseTile(c));
            }
        }

        if (m_tiles.size() != expectedSize) {
            throw std::runtime_error("Level tile count does not match grid dimensions");
        }

        const size_t cellCount = static_cast<size_t>(m_columns) * static_cast<size_t>(m_rows);
        m_walls.resize(cellCount, 0);
        m_tileAtCell.resize(cellCount, nullptr);
    }

    bool LevelGridComponent::IsWalkable(glm::ivec2 cell) const {
        if (!InBounds(cell)) {
            return false;
        }

        return m_walls[Index(cell)] == 0;
    }

    void LevelGridComponent::SetWall(glm::ivec2 cell, bool isWall) {
        if (!InBounds(cell)) {
            return;
        }

        m_walls[Index(cell)] = isWall ? 1 : 0;
    }

    glm::vec2 LevelGridComponent::CellToWorld(glm::ivec2 cell) const {
        return m_origin + glm::vec2(cell) * m_cellSize;
    }

    TileType LevelGridComponent::GetTile(int column, int row) const {
        if (column < 0 || column >= m_columns || row < 0 || row >= m_rows) {
            return TileType::Empty;
        }

        return m_tiles[static_cast<size_t>(row) * static_cast<size_t>(m_columns) + static_cast<size_t>(column)];
    }

    bool LevelGridComponent::HasWall(glm::ivec2 cell) const {
        return InBounds(cell) && m_tiles[Index(cell)] == TileType::Wall;
    }

    bool LevelGridComponent::IsBrick(glm::ivec2 cell) const {
        if (!InBounds(cell)) {
            return false;
        }

        const auto idx = Index(cell);
        return m_tiles[idx] == TileType::Brick && m_walls[idx] != 0 && m_tileAtCell[idx] != nullptr;
    }

    GameObject *LevelGridComponent::DestroyBrick(glm::ivec2 cell) {
        if (!InBounds(cell)) {
            return nullptr;
        }

        const auto idx = Index(cell);
        if (m_tiles[idx] != TileType::Brick || m_tileAtCell[idx] == nullptr) {
            return nullptr;
        }

        SetWall(cell, false);
        auto *tile = m_tileAtCell[idx];
        m_tileAtCell[idx] = nullptr;

        return tile;
    }

    void LevelGridComponent::SetTile(GameObject *tile, glm::ivec2 cell) {
        if (!InBounds(cell)) {
            return;
        }

        m_tileAtCell[Index(cell)] = tile;
    }

    bool LevelGridComponent::InBounds(glm::ivec2 cell) const {
        return cell.x >= 0 && cell.x < m_columns && cell.y >= 0 && cell.y < m_rows;
    }

    size_t LevelGridComponent::Index(glm::ivec2 cell) const {
        return static_cast<size_t>(cell.y) * static_cast<size_t>(m_columns) + static_cast<size_t>(cell.x);
    }
}
