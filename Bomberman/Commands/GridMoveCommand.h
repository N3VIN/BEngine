#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/GridMovementComponent.h"
#include <glm/glm.hpp>

namespace bomberman {
    class GridMoveCommand final : public bengine::GameObjectCommand {
    public:
        GridMoveCommand(bengine::GameObject *gameObject, glm::ivec2 direction)
            : bengine::GameObjectCommand(gameObject)
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
