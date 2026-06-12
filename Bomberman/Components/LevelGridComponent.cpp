#include "LevelGridComponent.h"
#include <stdexcept>
#include "Level/LevelData.h"

bomberman::TileType bomberman::LevelGridComponent::ParseTile(char tile) {
    switch (tile) {
        case '#':
            return TileType::Wall;
        case 'B':
        case 'X':
        case 'P':
        case 'F':
        case 'R':
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
    const LevelData data = LoadLevel(levelPath);
    m_columns = data.columns;
    m_rows = data.rows;
    m_cellSize = data.cellSize;
    m_origin = data.origin;

    const size_t expectedSize = static_cast<size_t>(m_columns) * static_cast<size_t>(m_rows);
    m_tiles.reserve(expectedSize);

    int rowIndex = 0;
    for (const auto &rowString: data.grid) {
        int columnIndex = 0;
        for (const char c: rowString) {
            const glm::ivec2 cell{columnIndex, rowIndex};
            switch (c) {
                case 'X':
                    m_exitCell = cell;
                    break;
                case 'P':
                    m_pickups.push_back({cell, PickupType::BombUp});
                    break;
                case 'F':
                    m_pickups.push_back({cell, PickupType::FlameUp});
                    break;
                case 'R':
                    m_pickups.push_back({cell, PickupType::Detonator});
                    break;
                default:
                    break;
            }

            m_tiles.push_back(ParseTile(c));
            ++columnIndex;
        }
        ++rowIndex;
    }

    if (m_tiles.size() != expectedSize) {
        throw std::runtime_error("Level tile count does not match grid dimensions");
    }

    m_walls.resize(expectedSize, 0);
    m_tileAtCell.resize(expectedSize, nullptr);
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
