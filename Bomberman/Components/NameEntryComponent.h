#pragma once
#include <array>
#include <functional>
#include <vector>
#include <SDL3/SDL_pixels.h>

#include "Components/Component.h"
#include "HighScores.h"

namespace bengine {
    class TextComponent;
}

namespace bomberman {
    class NameEntryComponent final : public bengine::Component {
    public:
        explicit NameEntryComponent(bengine::GameObject *parent);

        void AddSlot(bengine::TextComponent *label);
        void SetOnConfirm(std::function<void(std::array<char, highscores::NameLength>)> onConfirm);

        void ChangeLetter(int direction);
        void MoveSlot(int direction);
        void Confirm() const;

    private:
        void Refresh() const;

        std::array<char, highscores::NameLength> m_letters{'-', '-', '-'};
        std::vector<bengine::TextComponent *> m_labels{};
        size_t m_slot{0};
        std::function<void(std::array<char, highscores::NameLength>)> m_onConfirm{};
        SDL_Color m_normalColor{180, 180, 180, 255};
        SDL_Color m_highlightColor{255, 255, 80, 255};
    };
}
