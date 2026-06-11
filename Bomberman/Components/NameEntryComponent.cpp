#include "NameEntryComponent.h"

#include <string>

#include "Components/TextComponent.h"

bomberman::NameEntryComponent::NameEntryComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {}

void bomberman::NameEntryComponent::AddSlot(bengine::TextComponent *label) {
    m_labels.push_back(label);
    Refresh();
}

void bomberman::NameEntryComponent::SetOnConfirm(std::function<void(std::array<char, highscores::NameLength>)> onConfirm) {
    m_onConfirm = std::move(onConfirm);
}

void bomberman::NameEntryComponent::ChangeLetter(int direction) {
    if (m_slot >= m_letters.size()) {
        return;
    }

    constexpr int alphabet = 26;
    int letter = (m_letters[m_slot] - 'A' + direction) % alphabet;
    if (letter < 0) {
        letter += alphabet;
    }

    m_letters[m_slot] = static_cast<char>('A' + letter);
    Refresh();
}

void bomberman::NameEntryComponent::MoveSlot(int direction) {
    const int count = m_letters.size();
    if (count == 0) {
        return;
    }

    int next = (static_cast<int>(m_slot) + direction) % count;
    if (next < 0) {
        next += count;
    }

    m_slot = static_cast<size_t>(next);
    Refresh();
}

void bomberman::NameEntryComponent::Confirm() const {
    if (m_onConfirm) {
        m_onConfirm(m_letters);
    }
}

void bomberman::NameEntryComponent::Refresh() const {
    for (size_t i = 0; i < m_labels.size(); ++i) {
        m_labels[i]->SetText(std::string(1, m_letters[i]));
        m_labels[i]->SetColor(i == m_slot ? m_highlightColor : m_normalColor);
    }
}
