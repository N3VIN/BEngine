#include "EnemyStateComponent.h"
#include "GridMovementComponent.h"
#include "SpriteRendererComponent.h"
#include "States/EnemyStates.h"
#include "GameEvents.h"
#include "Level/Tileset.h"
#include "Patterns/ServiceLocator.h"
#include "Patterns/EventBus.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/GameObject.h"

bomberman::EnemyStateComponent::EnemyStateComponent(bengine::GameObject *parent, bengine::Scene *scene, EnemyType type)
    : bengine::Component(parent)
  , m_scene(scene)
  , m_movement(parent->GetComponent<GridMovementComponent>())
  , m_sprite(parent->GetComponent<SpriteRendererComponent>())
  , m_sprites(&GetTileset().GetEnemySprites(type))
  , m_points(GetEnemyStats(type).points) {
    m_currentState = MakeEnemyWalkState(m_movement->GetFacing(), *m_sprites);
    m_currentState->OnEnter(*this);

    m_explosionSub = bengine::ServiceLocator::GetEventBus().Subscribe<events::ExplosionAt>(
        [this](const events::ExplosionAt &event) {
            if (m_movement->GetCell() == event.cell) {
                m_killed = true;
            }
        }
    );
}

void bomberman::EnemyStateComponent::Update(float deltaTime) {
    if (m_killed) {
        m_killed = false;
        if (m_currentState->IsAlive()) {
            m_currentState->OnExit(*this);
            m_currentState = std::make_unique<EnemyDyingState>();
            m_currentState->OnEnter(*this);
            return;
        }
    }

    if (auto next = m_currentState->Update(*this, deltaTime)) {
        m_currentState->OnExit(*this);
        m_currentState = std::move(next);
        m_currentState->OnEnter(*this);
    }
}

void bomberman::EnemyStateComponent::Die() {
    bengine::ServiceLocator::GetEventBus().Broadcast(events::EnemyKilled{
            .enemy = GetParent(),
            .cell = m_movement->GetCell(),
            .points = m_points,
        }
    );

    m_scene->Remove(GetParent());
}
