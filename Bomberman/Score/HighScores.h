#pragma once
#include <array>
#include <vector>

namespace bomberman::highscores {
    constexpr int MaxEntries = 10;
    constexpr int NameLength = 3;

    struct Entry {
        std::array<char, NameLength> name{'-', '-', '-'};
        int score{0};
    };

    void Load();

    [[nodiscard]] const std::vector<Entry> &Table();
    [[nodiscard]] bool Qualifies(int score);
    void Insert(std::array<char, NameLength> name, int score);
}
