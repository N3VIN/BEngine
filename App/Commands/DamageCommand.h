#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/HealthComponent.h"

namespace dae {
    class DamageCommand final : public GameObjectCommand {
    public:
        explicit DamageCommand(GameObject *gameObject)
            : GameObjectCommand(gameObject) {
            m_healthComponent = gameObject->GetComponent<HealthComponent>();
        }

        void Execute() override {
            m_healthComponent->Die();
        }

    private:
        HealthComponent *m_healthComponent{nullptr};
    };
}
