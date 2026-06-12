#pragma once
#include <memory>

#include "Audio/IAudioService.h"
#include "Audio/NullAudioService.h"
#include "EventBus.h"

namespace bengine {
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

        static EventBus &GetEventBus() {
            if (!s_eventBus) {
                s_eventBus = std::make_unique<EventBus>();
            }

            return *s_eventBus;
        }

    private:
        inline static std::unique_ptr<IAudioService> s_audio;
        inline static std::unique_ptr<EventBus> s_eventBus; // I know this might be a weird place but I just wanted to avoid it as a singleton
    };
}
