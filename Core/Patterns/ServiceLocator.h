#pragma once
#include <memory>

#include "Audio/IAudioService.h"
#include "Audio/NullAudioService.h"

namespace dae {
    class ServiceLocator final {
    public:
        static IAudioService &GetAudioService() {
            if (!s_audio) {
                s_audio = std::make_unique<NullAudioService>();
            }

            return *s_audio;
        }

        static void RegisterAudioService(std::unique_ptr<IAudioService> service) {
            s_audio = service ? std::move(service) : std::make_unique<NullAudioService>();
        }

    private:
        inline static std::unique_ptr<IAudioService> s_audio;
    };
}
