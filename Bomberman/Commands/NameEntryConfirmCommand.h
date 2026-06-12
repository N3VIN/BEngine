#pragma once
#include "Patterns/ICommand.h"
#include "Components/Menu/NameEntryComponent.h"

namespace bomberman {
    class NameEntryConfirmCommand final : public bengine::ICommand {
    public:
        explicit NameEntryConfirmCommand(NameEntryComponent *entry)
            : m_entry(entry) {}

        void Execute() override {
            m_entry->Confirm();
        }

    private:
        NameEntryComponent *m_entry{nullptr};
    };
}
