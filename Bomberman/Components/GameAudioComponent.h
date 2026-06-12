#pragma once
#include "Utils.h"
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Patterns/ServiceLocator.h"

namespace bomberman {
    constexpr float sfxVolume{1.0f};
    constexpr float bgmVolume{0.5f};

    struct SoundEntry {
        bengine::SoundID id{};
        std::string_view file{};
    };

    constexpr std::array sounds{
        SoundEntry{bengine::Hash("bomb_place"), "Audio/bomb_lay.wav"},
        SoundEntry{bengine::Hash("explosion"), "Audio/bomb_explosion.wav"},
        SoundEntry{bengine::Hash("pickup"), "Audio/powerup.wav"},
        SoundEntry{bengine::Hash("player_death"), "Audio/bomberman_killed.wav"},
        SoundEntry{bengine::Hash("stage_start"), "Audio/stage_start.wav"},
        SoundEntry{bengine::Hash("stage_clear"), "Audio/stage_clear.wav"},
        SoundEntry{bengine::Hash("game_over"), "Audio/game_over.wav"},
        SoundEntry{bengine::Hash("step_horizontal"), "Audio/step_horizontal.wav"},
        SoundEntry{bengine::Hash("step_vertical"), "Audio/step_vertical.wav"},
    };

    constexpr SoundEntry bgm{bengine::Hash("bgm"), "Audio/main_BGM.wav"};

    inline void Play(bengine::SoundID id) {
        bengine::ServiceLocator::GetAudioService().PlayAudio(id, sfxVolume);
    }

    class GameAudioComponent final : public bengine::Component {
    public:
        explicit GameAudioComponent(bengine::GameObject *parent);
        ~GameAudioComponent() override;

    private:
        bengine::ScopedDelegate m_bombPlacedSub;
        bengine::ScopedDelegate m_bombDetonatedSub;
        bengine::ScopedDelegate m_pickupCollectedSub;
        bengine::ScopedDelegate m_levelCompletedSub;
        bengine::ScopedDelegate m_playerDiedSub;
        bengine::ScopedDelegate m_timeExpiredSub;
    };
}
