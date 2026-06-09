#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace bengine {
    using SoundID = uint32_t; // this is the 32 bit version of the hashing function

    class IAudioService {
    public:
        virtual ~IAudioService() = default;

        virtual void LoadAudio(SoundID id, fs::path path) = 0;
        virtual void PlayAudio(SoundID id, float volume, int loops = 0) = 0;
        virtual void StopAudio(SoundID id) = 0;

        virtual void SetMuted(bool muted) = 0;
        [[nodiscard]] virtual bool IsMuted() const = 0;
    };
}
