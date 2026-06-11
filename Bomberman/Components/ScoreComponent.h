#pragma once
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"

namespace bengine {
    class GameObject;
    class TextComponent;
}

namespace bomberman {
    class ScoreComponent final : public bengine::Component {
    public:
        ScoreComponent(bengine::GameObject *owner, bengine::GameObject *player, int playerIndex);

    private:
        void Refresh() const;

        bengine::GameObject *m_player{nullptr};
        int m_playerIndex{};
        bengine::TextComponent *m_text{nullptr};
        bengine::ScopedDelegate m_killedSub{};
    };
}
