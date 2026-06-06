#include "EnemyManagerComponent.h"
#include "EnemyFactory.h"
#include "SceneGraph/Scene.h"

bomberman::EnemyManagerComponent::EnemyManagerComponent(bengine::GameObject *parent, bengine::Scene *scene, LevelGridComponent *gridComponent)
    : bengine::Component(parent)
  , m_scene(scene)
  , m_gridComponent(gridComponent) {}

void bomberman::EnemyManagerComponent::RegisterPlayer(bengine::GameObject *player) {
    m_players.push_back(player); // teachers, you will see this a lot. we are not passing ownership. its just a pointer to the player.
}

void bomberman::EnemyManagerComponent::SpawnEnemy(EnemyType type, glm::ivec2 cell) {
    m_enemies.push_back(CreateEnemy(*m_scene, {m_gridComponent, type, cell, m_players}));
}
