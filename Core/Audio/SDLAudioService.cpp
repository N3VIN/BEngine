#include "SDLAudioService.h"

namespace bengine {
    struct LoadEvent {
        SoundID id;
        fs::path path;
    };

    struct PlayEvent {
        SoundID id;
        float volume;
    };

    struct StopEvent {
        SoundID id;
    };

    using AudioEvent = std::variant<LoadEvent, PlayEvent, StopEvent>;
}

class bengine::SDLAudioService::SDLAudioImpl {
public:
    SDLAudioImpl() {
        if (!MIX_Init()) {
            return;
        }

        m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!m_pMixer) {
            MIX_Quit();
            return;
        }

        m_thread = std::jthread([this](const std::stop_token &token) {
            Execute(token);
        });
    }

    ~SDLAudioImpl() {
        m_thread.request_stop();
        m_cv.notify_all(); // cv.wait does this for us but just incase..
        if (m_thread.joinable()) {
            m_thread.join(); // this must happen before we destroy the sdlmixer
        }

        for (const auto &track: m_track | std::views::values) {
            MIX_DestroyTrack(track);
        }

        for (const auto &audio: m_audio | std::views::values) {
            MIX_DestroyAudio(audio);
        }

        if (m_pMixer) {
            MIX_DestroyMixer(m_pMixer);
            MIX_Quit();
        }
    }

    void Enqueue(AudioEvent event) { {
            std::lock_guard lock(m_mutex);
            m_queue.push(std::move(event));
        }
        
        m_cv.notify_one();
    }

private:
    void Execute(const std::stop_token &token) {
        while (true) {
            AudioEvent event; {
                std::unique_lock lock(m_mutex);
                m_cv.wait(lock, token, [this] {
                    return !m_queue.empty();
                });

                if (token.stop_requested() && m_queue.empty()) {
                    return;
                }

                event = std::move(m_queue.front());
                m_queue.pop();
            }

            std::visit([this](auto &&e) {
                Process(e);
            }, event);
        }
    }

    void Process(const LoadEvent &event) {
        if (m_audio.contains(event.id)) {
            return;
        }

        const auto pathStr = event.path.string();
        auto *audio = MIX_LoadAudio(m_pMixer, pathStr.c_str(), false);
        if (!audio) {
            return;
        }

        auto *track = MIX_CreateTrack(m_pMixer);
        if (!track) {
            MIX_DestroyAudio(audio);
            return;
        }

        MIX_SetTrackAudio(track, audio);
        m_audio.emplace(event.id, audio);
        m_track.emplace(event.id, track);
    }

    void Process(const PlayEvent &event) {
        const auto it = m_track.find(event.id);
        if (it == m_track.end()) {
            return;
        }

        MIX_SetTrackGain(it->second, event.volume);
        MIX_PlayTrack(it->second, 0);
    }

    void Process(const StopEvent &event) {
        const auto it = m_track.find(event.id);
        if (it == m_track.end()) {
            return;
        }

        MIX_StopTrack(it->second, 0);
    }

    std::queue<AudioEvent> m_queue;
    std::unordered_map<SoundID, MIX_Audio *> m_audio;
    std::unordered_map<SoundID, MIX_Track *> m_track;
    MIX_Mixer *m_pMixer{nullptr};
    std::mutex m_mutex;
    std::condition_variable_any m_cv;
    std::jthread m_thread;
};

bengine::SDLAudioService::SDLAudioService()
    : pImpl(std::make_unique<SDLAudioImpl>()) {}

bengine::SDLAudioService::~SDLAudioService() = default;
bengine::SDLAudioService::SDLAudioService(SDLAudioService &&) noexcept = default;
bengine::SDLAudioService &bengine::SDLAudioService::operator=(SDLAudioService &&) noexcept = default;

void bengine::SDLAudioService::LoadAudio(SoundID id, fs::path path) {
    pImpl->Enqueue(LoadEvent{id, std::move(path)});
}

void bengine::SDLAudioService::PlayAudio(SoundID id, float volume) {
    pImpl->Enqueue(PlayEvent{id, volume});
}

void bengine::SDLAudioService::StopAudio(SoundID id) {
    pImpl->Enqueue(StopEvent{id});
}
