#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/HealthComponent.h"

namespace app {
    class DamageCommand final : public bengine::GameObjectCommand {
    public:
        explicit DamageCommand(bengine::GameObject *gameObject)
            : bengine::GameObjectCommand(gameObject) {
            m_healthComponent = gameObject->GetComponent<HealthComponent>();
        }

        void Execute() override {
            m_healthComponent->Die();
        }

    private:
        HealthComponent *m_healthComponent{nullptr};
    };
}
