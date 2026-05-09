#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/GridMovementComponent.h"
#include <glm/glm.hpp>

namespace dae {
    class GridMoveCommand final : public GameObjectCommand {
    public:
        GridMoveCommand(GameObject *gameObject, glm::ivec2 direction)
            : GameObjectCommand(gameObject)
          , m_direction(direction) {}

        void Execute() override {
            if (auto *movementComponent = GetGameObject()->GetComponent<GridMovementComponent>()) {
                movementComponent->SetDesiredDirection(m_direction);
            }
        }

    private:
        glm::ivec2 m_direction;
    };
}
