#include "GameAudioComponent.h"

#include "GameEvents.h"
#include "utils.h"
#include "Audio/IAudioService.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "Renderer/ResourceManager.h"

bomberman::GameAudioComponent::GameAudioComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {
    auto &audio = bengine::ServiceLocator::GetAudioService();
    const auto &dataPath = bengine::ResourceManager::GetInstance().GetDataPath();
    for (const auto &[id, file]: SOUNDS) {
        audio.LoadAudio(id, dataPath / file);
    }

    auto &bus = bengine::ServiceLocator::GetEventBus();

    m_bombPlacedSub = bus.Subscribe<events::BombPlaced>(
        [](const events::BombPlaced &) { Play(utils::Hash("bomb_place")); }
    );
    m_bombDetonatedSub = bus.Subscribe<events::BombDetonated>(
        [](const events::BombDetonated &) { Play(utils::Hash("explosion")); }
    );
    m_pickupCollectedSub = bus.Subscribe<events::PickupCollected>(
        [](const events::PickupCollected &) { Play(utils::Hash("pickup")); }
    );
}
