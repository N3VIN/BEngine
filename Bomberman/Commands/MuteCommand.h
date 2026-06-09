#pragma once
#include "Patterns/ICommand.h"
#include "Patterns/ServiceLocator.h"

namespace bomberman {
    class MuteCommand final : public bengine::ICommand {
    public:
        void Execute() override {
            auto &audio = bengine::ServiceLocator::GetAudioService();
            audio.SetMuted(!audio.IsMuted());
        }
    };
}
