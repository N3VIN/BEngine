#pragma once
#include <cstddef>
#include <cstdint>
#include <array>

namespace bomberman {
    enum class EnemyType : uint8_t {
        Balloom,
        Onil,
        Dall,
        Minvo
    };

    inline constexpr size_t EnemyTypeCount{4};

    enum class SightAxes : uint8_t {
        None = 0,
        Horizontal = 1,
        Vertical = 2,
        Both = 3
    };

    [[nodiscard]] constexpr bool SeesAlongRow(SightAxes axes) {
        return (static_cast<uint8_t>(axes) & static_cast<uint8_t>(SightAxes::Horizontal)) != 0;
    }

    [[nodiscard]] constexpr bool SeesAlongColumn(SightAxes axes) {
        return (static_cast<uint8_t>(axes) & static_cast<uint8_t>(SightAxes::Vertical)) != 0;
    }

    //type object
    struct EnemyStats {
        float cellsPerSecond{};
        int points{};
        SightAxes sight{SightAxes::None};
        float minInterval{};
        float maxInterval{};
    };

    [[nodiscard]] inline const EnemyStats &GetEnemyStats(EnemyType type) {
        static constexpr std::array<EnemyStats, EnemyTypeCount> table{
            {
                {3.0f, 100, SightAxes::None, 2.0f, 4.0f},
                {4.0f, 200, SightAxes::Vertical, 0.8f, 3.2f},
                {4.0f, 400, SightAxes::Horizontal, 0.8f, 3.2f},
                {6.0f, 800, SightAxes::Both, 0.5f, 2.0f},
            }
        };

        return table[static_cast<size_t>(type)];
    }
}
