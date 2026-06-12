#pragma once
#include <print>

#include "IAudioService.h"

namespace bengine {
    class NullAudioService final : public IAudioService {
    public:
        void LoadAudio(SoundID /*id*/, fs::path /*path*/) override {
            std::println("Load Audio but this is null!!");
        }

        // they print nothing since they polute the console
        void PlayAudio(SoundID /*id*/, float /*volume*/, int /*loops*/) override {}
        void StopAudio(SoundID /*id*/) override {}
        void SetMuted(bool /*muted*/) override {}
        [[nodiscard]] bool IsMuted() const override { return true; }
    };
}
