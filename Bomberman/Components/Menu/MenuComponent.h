#pragma once
#include <functional>
#include <vector>

#include "Components/Component.h"

namespace bengine {
    class SpriteTextComponent;
}

namespace bomberman {
    class MenuComponent final : public bengine::Component {
    public:
        explicit MenuComponent(bengine::GameObject *parent);

        void AddItem(bengine::SpriteTextComponent *text, std::function<void()> onSelect);
        void MoveSelection(int direction);
        void Confirm() const;

    private:
        struct Item {
            bengine::SpriteTextComponent *text{nullptr};
            std::function<void()> onSelect{};
        };

        void Refresh() const;

        std::vector<Item> m_items{};
        size_t m_selected{0};
    };
}
