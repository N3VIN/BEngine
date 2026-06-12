#include "GameAudioComponent.h"

#include "Gameplay/GameEvents.h"
#include "Utils.h"
#include "Audio/IAudioService.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "Renderer/ResourceManager.h"

bomberman::GameAudioComponent::GameAudioComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {
    auto &audio = bengine::ServiceLocator::GetAudioService();
    const auto &dataPath = bengine::ResourceManager::GetInstance().GetDataPath();
    for (const auto &[id, file]: sounds) {
        audio.LoadAudio(id, dataPath / file);
    }

    audio.LoadAudio(bgm.id, dataPath / bgm.file);
    audio.PlayAudio(bgm.id, bgmVolume, -1);
    Play(bengine::Hash("stage_start"));

    auto &bus = bengine::ServiceLocator::GetEventBus();

    m_bombPlacedSub = bus.Subscribe<events::BombPlaced>(
        [](const events::BombPlaced &) { Play(bengine::Hash("bomb_place")); }
    );

    m_bombDetonatedSub = bus.Subscribe<events::BombDetonated>(
        [](const events::BombDetonated &) { Play(bengine::Hash("explosion")); }
    );

    m_pickupCollectedSub = bus.Subscribe<events::PickupCollected>(
        [](const events::PickupCollected &) { Play(bengine::Hash("pickup")); }
    );

    m_levelCompletedSub = bus.Subscribe<events::LevelCompleted>(
        [](const events::LevelCompleted &) { Play(bengine::Hash("stage_clear")); }
    );

    m_playerDiedSub = bus.Subscribe<events::PlayerDied>(
        [](const events::PlayerDied &) { Play(bengine::Hash("game_over")); }
    );

    m_timeExpiredSub = bus.Subscribe<events::TimeExpired>(
        [](const events::TimeExpired &) { Play(bengine::Hash("game_over")); }
    );
}

bomberman::GameAudioComponent::~GameAudioComponent() {
    bengine::ServiceLocator::GetAudioService().StopAudio(bgm.id);
}
