#pragma once
#include <memory>

#include "CoopMode.h"
#include "GameMode.h"
#include "SoloMode.h"
#include "VersusMode.h"

namespace bomberman {
    class IGameMode;

    inline std::unique_ptr<IGameMode> CreateGameMode(GameMode mode) {
        switch (mode) {
            case GameMode::Solo:
                return std::make_unique<SoloMode>();
            case GameMode::Coop:
                return std::make_unique<CoopMode>();
            case GameMode::Versus:
                return std::make_unique<VersusMode>();
        }

        return std::make_unique<SoloMode>();
    }
}
