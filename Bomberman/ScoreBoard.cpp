#include "ScoreBoard.h"

#include <vector>

namespace {
    std::vector<int> &Scores() {
        static std::vector<int> scores{}; // core guideline use local static instead of singleton
        return scores;
    }

    [[nodiscard]] bool InRange(int playerIndex) {
        return playerIndex >= 0 && static_cast<size_t>(playerIndex) < Scores().size();
    }
}

void bomberman::scoreboard::Reset(int playerCount) {
    Scores().assign(static_cast<size_t>(playerCount < 0 ? 0 : playerCount), 0);
}

void bomberman::scoreboard::Add(int playerIndex, int points) {
    if (InRange(playerIndex)) {
        Scores()[static_cast<size_t>(playerIndex)] += points;
    }
}

int bomberman::scoreboard::Get(int playerIndex) {
    return InRange(playerIndex) ? Scores()[static_cast<size_t>(playerIndex)] : 0;
}

int bomberman::scoreboard::PlayerCount() {
    return static_cast<int>(Scores().size());
}
