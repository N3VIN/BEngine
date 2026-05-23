#pragma once
#include "Patterns/ICommand.h"
#include "SceneGraph/GameObject.h"
#include "Components/PickupComponent.h"

namespace app {
    class ScoreCommand final : public bengine::GameObjectCommand {
    public:
        ScoreCommand(bengine::GameObject *gameObject, int score)
            : bengine::GameObjectCommand(gameObject)
          , m_score(score) {
            m_pickupComponent = gameObject->GetComponent<PickupComponent>();
        }

        void Execute() override {
            m_pickupComponent->OnPickup(m_score);
        }

    private:
        int m_score;
        PickupComponent *m_pickupComponent{nullptr};
    };
}
