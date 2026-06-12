#include "ScoreComponent.h"

#include <string>

#include "ScoreBoard.h"
#include "GameEvents.h"
#include "SceneGraph/GameObject.h"
#include "Components/SpriteTextComponent.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"

bomberman::ScoreComponent::ScoreComponent(bengine::GameObject *owner, bengine::GameObject *player, int playerIndex)
    : bengine::Component(owner)
  , m_player(player)
  , m_playerIndex(playerIndex)
  , m_text(owner->GetComponent<bengine::SpriteTextComponent>()) {
    Refresh();

    m_killedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::EnemyKilled>(
        [this](const events::EnemyKilled &event) {
            if (event.killer == m_player) {
                scoreboard::Add(m_playerIndex, event.points); // we use the player index to update the scoreboard
                Refresh();
            }
        }
    );
}

void bomberman::ScoreComponent::Refresh() const {
    m_text->SetText("SCORE " + std::to_string(scoreboard::Get(m_playerIndex)));
}
