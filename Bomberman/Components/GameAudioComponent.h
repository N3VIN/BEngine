#pragma once
#include "utils.h"
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Patterns/ServiceLocator.h"

namespace bomberman {
    constexpr float SFX_VOLUME{1.0f};
    constexpr float BGM_VOLUME{0.5f};

    struct SoundEntry {
        bengine::SoundID id{};
        std::string_view file{};
    };

    constexpr std::array SOUNDS{
        SoundEntry{utils::Hash("bomb_place"), "Audio/bomb_lay.wav"},
        SoundEntry{utils::Hash("explosion"), "Audio/bomb_explosion.wav"},
        SoundEntry{utils::Hash("pickup"), "Audio/powerup.wav"},
        SoundEntry{utils::Hash("player_death"), "Audio/bomberman_killed.wav"},
        SoundEntry{utils::Hash("stage_start"), "Audio/stage_start.wav"},
        SoundEntry{utils::Hash("stage_clear"), "Audio/stage_clear.wav"},
        SoundEntry{utils::Hash("game_over"), "Audio/game_over.wav"},
    };

    constexpr SoundEntry BGM{utils::Hash("bgm"), "Audio/main_BGM.wav"};

    inline void Play(bengine::SoundID id) {
        bengine::ServiceLocator::GetAudioService().PlayAudio(id, SFX_VOLUME);
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
