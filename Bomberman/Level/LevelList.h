#pragma once
#include <string>
#include <vector>

namespace bomberman {
    inline const std::vector<std::string> &GetLevelList() noexcept {
        static const std::vector<std::string> levels{
            "Levels/level1.json",
            // more levels here when we add them
        };

        return levels;
    }
}
