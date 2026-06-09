#pragma once
#include <functional>
#include <vector>
#include <SDL3/SDL_pixels.h>

#include "Components/Component.h"

namespace bengine {
    class TextComponent;
}

namespace bomberman {
    class MenuComponent final : public bengine::Component {
    public:
        explicit MenuComponent(bengine::GameObject *parent);

        void AddItem(bengine::TextComponent *text, std::function<void()> onSelect);
        void MoveSelection(int direction);
        void Confirm() const;

    private:
        struct Item {
            bengine::TextComponent *text{nullptr};
            std::function<void()> onSelect{};
        };

        void Refresh() const;

        std::vector<Item> m_items{};
        size_t m_selected{0};
        SDL_Color m_normalColor{180, 180, 180, 255}; // TODO: make configurable
        SDL_Color m_highlightColor{255, 255, 80, 255};
    };
}
