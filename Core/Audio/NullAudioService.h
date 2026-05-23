#pragma once
#include <print>

#include "IAudioService.h"

namespace bengine {
    class NullAudioService final : public IAudioService {
    public:
        void LoadAudio(SoundID /*id*/, fs::path /*path*/) override {
            std::println("Load Audio but this is null!!");
        }

        void PlayAudio(SoundID /*id*/, float /*volume*/) override {
            std::println("Play Audio but this is null!!");
        }

        void StopAudio(SoundID /*id*/) override {
            std::println("Stop Audio but this is null!!");
        }
    };
}
