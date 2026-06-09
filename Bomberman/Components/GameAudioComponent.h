#pragma once
#include "utils.h"
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Patterns/ServiceLocator.h"

namespace bomberman {
    constexpr float SFX_VOLUME{1.0f};

    struct SoundEntry {
        bengine::SoundID id{};
        std::string_view file{};
    };

    constexpr std::array SOUNDS{
        SoundEntry{utils::Hash("bomb_place"), "Audio/bomb_lay.wav"},
        SoundEntry{utils::Hash("explosion"), "Audio/bomb_explosion.wav"},
        SoundEntry{utils::Hash("pickup"), "Audio/powerup.wav"},
        SoundEntry{utils::Hash("player_death"), "Audio/bomberman_killed.wav"},
    };

    inline void Play(bengine::SoundID id) {
        bengine::ServiceLocator::GetAudioService().PlayAudio(id, SFX_VOLUME);
    }

    class GameAudioComponent final : public bengine::Component {
    public:
        explicit GameAudioComponent(bengine::GameObject *parent);

    private:
        bengine::ScopedDelegate m_bombPlacedSub;
        bengine::ScopedDelegate m_bombDetonatedSub;
        bengine::ScopedDelegate m_pickupCollectedSub;
    };
}
