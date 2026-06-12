#pragma once
#include "Patterns/ICommand.h"
#include "Components/Menu/MenuComponent.h"

namespace bomberman {
    class MenuNavigateCommand final : public bengine::ICommand {
    public:
        MenuNavigateCommand(MenuComponent *menu, int direction)
            : m_menu(menu)
          , m_direction(direction) {}

        void Execute() override {
            m_menu->MoveSelection(m_direction);
        }

    private:
        MenuComponent *m_menu{nullptr};
        int m_direction{0};
    };
}
