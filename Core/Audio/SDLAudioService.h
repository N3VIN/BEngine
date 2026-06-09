#pragma once
#include "IAudioService.h"

namespace bengine {
    class SDLAudioService final : public IAudioService {
    public:
        SDLAudioService();
        ~SDLAudioService() override;

        SDLAudioService(const SDLAudioService &) = delete;
        SDLAudioService &operator=(const SDLAudioService &) = delete;
        SDLAudioService(SDLAudioService &&) noexcept;
        SDLAudioService &operator=(SDLAudioService &&) noexcept;

        void LoadAudio(SoundID id, fs::path path) override;
        void PlayAudio(SoundID id, float volume) override;
        void StopAudio(SoundID id) override;

        void SetMuted(bool muted) override;
        [[nodiscard]] bool IsMuted() const override;

    private:
        class SDLAudioImpl;
        std::unique_ptr<SDLAudioImpl> pImpl;
    };
}
