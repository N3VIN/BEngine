#pragma once
#include <SDL3/SDL_events.h>

#include "Patterns/ICommand.h"

namespace dae {
    class QuitCommand final : public ICommand {
    public:
        void Execute() override {
            SDL_Event event{};
            event.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&event);
        }
    };
}
