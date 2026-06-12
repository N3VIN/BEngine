#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace bengine {
    class GameObject;
    class SpriteTextComponent;
}

namespace bomberman {
    class LivesComponent final : public bengine::Component {
    public:
        LivesComponent(bengine::GameObject *owner, bengine::GameObject *player);

    private:
        void Refresh(int lives) const;

        bengine::SpriteTextComponent *m_text{nullptr};
        bengine::ScopedDelegate m_damagedSub{};
    };
}
