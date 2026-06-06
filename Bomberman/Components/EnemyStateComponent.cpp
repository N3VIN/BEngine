#include "EnemyStateComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "States/EnemyStates.h"
#include "Level/Tileset.h"
#include "SceneGraph/GameObject.h"

bomberman::EnemyStateComponent::EnemyStateComponent(bengine::GameObject *parent, EnemyType type)
    : bengine::Component(parent)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<SpriteRendererComponent>())
  , m_sprites(&GetTileset().GetEnemySprites(type)) {
    m_currentState = MakeEnemyWalkState(m_movement->GetFacing(), *m_sprites);
    m_currentState->OnEnter(*this);
}

void bomberman::EnemyStateComponent::Update(float deltaTime) {
    if (auto next = m_currentState->Update(*this, deltaTime)) {
        m_currentState->OnExit(*this);
        m_currentState = std::move(next);
        m_currentState->OnEnter(*this);
    }
}
