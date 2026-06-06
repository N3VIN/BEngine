#pragma once
#include <string>
#include <vector>

namespace bomberman {
    inline const std::vector<std::string> &GetLevelList() noexcept {
        static const std::vector<std::string> levels{
            "Levels/level1.json",
            "Levels/level2.json",
            "Levels/level3.json",
            "Levels/level4.json",
        };

        return levels;
    }
}
