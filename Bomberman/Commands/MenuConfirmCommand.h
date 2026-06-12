#pragma once
#include "Patterns/ICommand.h"
#include "Components/Menu/MenuComponent.h"

namespace bomberman {
    class MenuConfirmCommand final : public bengine::ICommand {
    public:
        explicit MenuConfirmCommand(MenuComponent *menu)
            : m_menu(menu) {}

        void Execute() override {
            m_menu->Confirm();
        }

    private:
        MenuComponent *m_menu{nullptr};
    };
}
