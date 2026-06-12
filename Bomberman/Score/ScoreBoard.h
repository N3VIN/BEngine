#pragma once

namespace bomberman::scoreboard {
    void Reset(int playerCount);
    void Add(int playerIndex, int points);

    [[nodiscard]] int Get(int playerIndex);
    [[nodiscard]] int PlayerCount();
}
