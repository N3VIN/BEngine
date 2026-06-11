#include "HighScores.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace {
    const std::filesystem::path &FilePath() {
        static const std::filesystem::path path{"highscores.bomb"};
        return path;
    }

    std::vector<bomberman::highscores::Entry> &Entries() {
        static std::vector<bomberman::highscores::Entry> entries{};
        return entries;
    }

    void Sort() {
        std::ranges::stable_sort(Entries(), [](const bomberman::highscores::Entry &left, const bomberman::highscores::Entry &right) {
                                     return left.score > right.score;
                                 }
        );
    }

    void Save() {
        std::ofstream file(FilePath(), std::ios::binary | std::ios::trunc);
        if (!file) {
            return;
        }

        const auto &entries = Entries();
        const int count = static_cast<int>(entries.size());
        file.write(reinterpret_cast<const char *>(&count), sizeof(count));
        for (const auto &[name, score]: entries) {
            file.write(name.data(), bomberman::highscores::NameLength);
            file.write(reinterpret_cast<const char *>(&score), sizeof(score));
        }
    }
}

void bomberman::highscores::Load() {
    Entries().clear();

    std::ifstream file(FilePath(), std::ios::binary);
    if (!file) {
        return;
    }

    int count{0};
    file.read(reinterpret_cast<char *>(&count), sizeof(count));
    if (!file) {
        return;
    }

    count = std::clamp(count, 0, MaxEntries);
    for (int i = 0; i < count; ++i) {
        Entry entry{};
        file.read(entry.name.data(), NameLength);
        file.read(reinterpret_cast<char *>(&entry.score), sizeof(entry.score));
        if (!file) {
            break;
        }

        Entries().push_back(entry);
    }

    Sort();
}

const std::vector<bomberman::highscores::Entry> &bomberman::highscores::Table() {
    return Entries();
}

bool bomberman::highscores::Qualifies(int score) {
    if (score <= 0) {
        return false;
    }

    const auto &entries = Entries();
    return entries.size() < static_cast<size_t>(MaxEntries) || score > entries.back().score;
}

void bomberman::highscores::Insert(std::array<char, NameLength> name, int score) {
    Entries().push_back({name, score});
    Sort();

    if (Entries().size() > static_cast<size_t>(MaxEntries)) {
        Entries().resize(MaxEntries);
    }

    Save();
}
