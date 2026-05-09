#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Engine/TimeManager.h"
#include <glm/glm.hpp>

namespace dae {
    class MoveCommand final : public GameObjectCommand {
    public:
        MoveCommand(GameObject *gameObject, const glm::vec2 &direction, float speed)
            : GameObjectCommand(gameObject)
          , m_direction(direction)
          , m_speed(speed) {}

        void Execute() override {
            auto pos = glm::vec2(GetGameObject()->GetLocalPosition());
            pos += m_direction * m_speed * Time::GetInstance().deltaTime;
            GetGameObject()->SetLocalPosition(pos);
        }

    private:
        glm::vec2 m_direction;
        float m_speed;
    };
}
