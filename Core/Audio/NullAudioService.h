#pragma once
#include <print>

#include "IAudioService.h"

namespace bengine {
    class NullAudioService final : public IAudioService {
    public:
        void LoadAudio(SoundID /*id*/, fs::path /*path*/) override {
            std::println("Load Audio but this is null!!");
        }

        void PlayAudio(SoundID /*id*/, float /*volume*/, int /*loops*/) override {
            std::println("Play Audio but this is null!!");
        }

        void StopAudio(SoundID /*id*/) override {
            std::println("Stop Audio but this is null!!");
        }

        void SetMuted(bool /*muted*/) override {
            std::println("Set Muted but this is null!!");
        }

        [[nodiscard]] bool IsMuted() const override {
            std::println("Is Muted but this is null!!");
            return true;
        }
    };
}
