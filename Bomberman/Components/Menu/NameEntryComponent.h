#pragma once
#include <array>
#include <functional>
#include <vector>

#include "Components/Component.h"
#include "Score/HighScores.h"

namespace bengine {
    class SpriteTextComponent;
}

namespace bomberman {
    class NameEntryComponent final : public bengine::Component {
    public:
        explicit NameEntryComponent(bengine::GameObject *parent);

        void AddSlot(bengine::SpriteTextComponent *label);
        void SetOnConfirm(std::function<void(std::array<char, highscores::NameLength>)> onConfirm);

        void ChangeLetter(int direction);
        void MoveSlot(int direction);
        void Confirm() const;

    private:
        void Refresh() const;

        std::array<char, highscores::NameLength> m_letters{'-', '-', '-'};
        std::vector<bengine::SpriteTextComponent *> m_labels{};
        size_t m_slot{0};
        std::function<void(std::array<char, highscores::NameLength>)> m_onConfirm{};
    };
}
