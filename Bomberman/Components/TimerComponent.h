#pragma once
#include "Components/Component.h"
#include "Timer.h"

namespace bengine {
    class GameObject;
    class SpriteTextComponent;
}

namespace bomberman {
    class TimerComponent final : public bengine::Component {
    public:
        TimerComponent(bengine::GameObject *owner, float seconds);

        void Update(float deltaTime) override;

    private:
        void Refresh(int seconds) const;

        bengine::SpriteTextComponent *m_text{nullptr};
        bengine::Timer m_timer{};
        int m_displayedSeconds{-1};
        bool m_expired{false};
    };
}
