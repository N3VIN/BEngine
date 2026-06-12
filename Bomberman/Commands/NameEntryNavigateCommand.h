#pragma once
#include "Patterns/ICommand.h"
#include "Components/Menu/NameEntryComponent.h"

namespace bomberman {
    class NameEntryNavigateCommand final : public bengine::ICommand {
    public:
        NameEntryNavigateCommand(NameEntryComponent *entry, int letterDir, int slotDir)
            : m_entry(entry)
          , m_letterDir(letterDir)
          , m_slotDir(slotDir) {}

        void Execute() override {
            if (m_letterDir != 0) {
                m_entry->ChangeLetter(m_letterDir);
            }
            if (m_slotDir != 0) {
                m_entry->MoveSlot(m_slotDir);
            }
        }

    private:
        NameEntryComponent *m_entry{nullptr};
        int m_letterDir{0};
        int m_slotDir{0};
    };
}
