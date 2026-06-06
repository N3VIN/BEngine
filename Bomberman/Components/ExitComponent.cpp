#include "ExitComponent.h"
#include "LevelGridComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "Level/Tileset.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include <algorithm>

bomberman::ExitComponent::ExitComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent)
    : bengine::Component(parent)
  , m_grid(gridComponent)
  , m_exitCell(gridComponent->GetExitCell()) {
    auto &bus = bengine::ServiceLocator::GetEventBus();

    m_brickDestroyedSub = bus.Subscribe<events::BrickDestroyed>(
        [this](const events::BrickDestroyed &event) {
            if (!m_revealed && event.cell == m_exitCell) {
                RevealExit();
            }
        }
    );

    m_enemiesDefeatedSub = bus.Subscribe<events::AllEnemiesDefeated>(
        [this](const events::AllEnemiesDefeated &) {
            m_enemiesCleared = true;
        }
    );
}

void bomberman::ExitComponent::Update(float) {
    if (m_completed || !m_revealed || !m_enemiesCleared) {
        return;
    }

    const bool playerOnExit = std::ranges::any_of(
        m_players, [this](const GridMovementComponent *player) {
            return player->GetCell() == m_exitCell;
        }
    );

    if (playerOnExit) {
        m_completed = true;
        bengine::ServiceLocator::GetEventBus().Broadcast(events::LevelCompleted{});
    }
}

void bomberman::ExitComponent::RegisterPlayer(const bengine::GameObject *player) {
    m_players.push_back(player->GetComponent<GridMovementComponent>());
}

void bomberman::ExitComponent::RevealExit() {
    auto exitGO = std::make_unique<bengine::GameObject>();
    exitGO->SetLocalPosition(m_grid->CellToWorld(m_exitCell));
    exitGO->AddComponent<SpriteRendererComponent>(SpriteType::Exit);
    bengine::GetActiveScene()->Add(std::move(exitGO));
    m_revealed = true;
}
