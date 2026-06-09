#include "MenuComponent.h"

#include "Components/TextComponent.h"

bomberman::MenuComponent::MenuComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {}

void bomberman::MenuComponent::AddItem(bengine::TextComponent *text, std::function<void()> onSelect) {
    m_items.push_back({text, std::move(onSelect)});
    Refresh();
}

void bomberman::MenuComponent::MoveSelection(int direction) {
    if (m_items.empty()) {
        return;
    }

    const int count = static_cast<int>(m_items.size());
    int next = (static_cast<int>(m_selected) + direction) % count;
    if (next < 0) {
        next += count;
    }

    m_selected = static_cast<size_t>(next);
    Refresh();
}

void bomberman::MenuComponent::Confirm() const {
    if (m_items.empty()) {
        return;
    }

    if (const auto &onSelect = m_items[m_selected].onSelect) {
        onSelect();
    }
}

void bomberman::MenuComponent::Refresh() const {
    for (size_t i = 0; i < m_items.size(); ++i) {
        m_items[i].text->SetColor(i == m_selected ? m_highlightColor : m_normalColor);
    }
}
