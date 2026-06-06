#include "EnemyManagerComponent.h"
#include "EnemyFactory.h"
#include "HazardComponent.h"
#include "GameEvents.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"

bomberman::EnemyManagerComponent::EnemyManagerComponent(bengine::GameObject *parent, LevelGridComponent *gridComponent, HazardComponent *hazardComponent)
    : bengine::Component(parent)
  , m_gridComponent(gridComponent)
  , m_hazardComponent(hazardComponent) {
    m_killedSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::EnemyKilled>(
        [this](const events::EnemyKilled &event) {
            std::erase(m_enemies, event.enemy);
        }
    );
}

void bomberman::EnemyManagerComponent::RegisterPlayer(bengine::GameObject *player) {
    m_players.push_back(player); // teachers, you will see this a lot. we are not passing ownership. its just a pointer to the player.
}

void bomberman::EnemyManagerComponent::SpawnEnemy(EnemyType type, glm::ivec2 cell) {
    auto *enemy = CreateEnemy(*bengine::GetActiveScene(), {m_gridComponent, type, cell, &m_players});
    m_enemies.push_back(enemy);
    m_hazardComponent->RegisterEnemy(enemy);
}
