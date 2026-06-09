#pragma once
#include "Patterns/ICommand.h"
#include "Components/MenuComponent.h"

namespace bomberman {
    class MenuNavigateCommand final : public bengine::ICommand {
    public:
        MenuNavigateCommand(MenuComponent *menu, int direction)
            : m_menu(menu)
          , m_delta(direction) {}

        void Execute() override {
            m_menu->MoveSelection(m_delta);
        }

    private:
        MenuComponent *m_menu{nullptr};
        int m_delta{0};
    };
}
