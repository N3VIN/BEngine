#include "LevelGridComponent.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

bomberman::TileType bomberman::LevelGridComponent::ParseTile(char tile) {
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
        case 'a':
            return TileType::BalloomSpawn;
        case 'o':
            return TileType::OnilSpawn;
        case 'd':
            return TileType::DallSpawn;
        case 'm':
            return TileType::MinvoSpawn;
        default:
            return TileType::Empty;
    }
}

bomberman::LevelGridComponent::LevelGridComponent(bengine::GameObject *parent, const fs::path &levelPath)
    : bengine::Component(parent) {
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

bool bomberman::LevelGridComponent::IsWalkable(glm::ivec2 cell) const {
    if (!InBounds(cell)) {
        return false;
    }

    return m_walls[Index(cell)] == 0;
}

bool bomberman::LevelGridComponent::HasClearPath(glm::ivec2 origin, glm::ivec2 target) const {
    const auto stepDirection = glm::sign(target - origin);
    for (auto current = origin + stepDirection; current != target; current += stepDirection) {
        if (!IsWalkable(current)) {
            return false;
        }
    }

    return true;
}

void bomberman::LevelGridComponent::SetWall(glm::ivec2 cell, bool isWall) {
    if (!InBounds(cell)) {
        return;
    }

    m_walls[Index(cell)] = isWall ? 1 : 0;
}

glm::vec2 bomberman::LevelGridComponent::CellToWorld(glm::ivec2 cell) const {
    return m_origin + glm::vec2(cell) * m_cellSize;
}

bomberman::TileType bomberman::LevelGridComponent::GetTile(int column, int row) const {
    if (column < 0 || column >= m_columns || row < 0 || row >= m_rows) {
        return TileType::Empty;
    }

    return m_tiles[static_cast<size_t>(row) * static_cast<size_t>(m_columns) + static_cast<size_t>(column)];
}

bool bomberman::LevelGridComponent::HasWall(glm::ivec2 cell) const {
    return InBounds(cell) && m_tiles[Index(cell)] == TileType::Wall;
}

bool bomberman::LevelGridComponent::IsBrick(glm::ivec2 cell) const {
    if (!InBounds(cell)) {
        return false;
    }

    const auto idx = Index(cell);
    return m_tiles[idx] == TileType::Brick && m_walls[idx] != 0 && m_tileAtCell[idx] != nullptr;
}

bengine::GameObject *bomberman::LevelGridComponent::DestroyBrick(glm::ivec2 cell) {
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

void bomberman::LevelGridComponent::SetTile(bengine::GameObject *tile, glm::ivec2 cell) {
    if (!InBounds(cell)) {
        return;
    }

    m_tileAtCell[Index(cell)] = tile;
}

bool bomberman::LevelGridComponent::InBounds(glm::ivec2 cell) const {
    return cell.x >= 0 && cell.x < m_columns && cell.y >= 0 && cell.y < m_rows;
}

size_t bomberman::LevelGridComponent::Index(glm::ivec2 cell) const {
    return static_cast<size_t>(cell.y) * static_cast<size_t>(m_columns) + static_cast<size_t>(cell.x);
}
