#pragma once
#include "Patterns/IObserver.h"
#include "Components/Component.h"

namespace app {
    class PickupComponent;

    class SteamAchievementComponent final : public bengine::Component, public bengine::IObserver {
    public:
        ~SteamAchievementComponent() override;
        explicit SteamAchievementComponent(bengine::GameObject *owner);
        void OnNotify(bengine::GameObject *gameObject, bengine::GameEvent event) override;
        static constexpr int SCORE_TO_WIN{500};

    private:
        PickupComponent *m_pickupComponent{nullptr};
        bool m_achieved{false};
    };
}
