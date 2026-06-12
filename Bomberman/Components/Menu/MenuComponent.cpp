#include "MenuComponent.h"
#include "Components/SpriteTextComponent.h"
#include "UI/UIColors.h"
#include "utils.h"

bomberman::MenuComponent::MenuComponent(bengine::GameObject *parent)
    : bengine::Component(parent) {}

void bomberman::MenuComponent::AddItem(bengine::SpriteTextComponent *text, std::function<void()> onSelect) {
    m_items.push_back({text, std::move(onSelect)});
    Refresh();
}

void bomberman::MenuComponent::MoveSelection(int direction) {
    if (m_items.empty()) {
        return;
    }

    const int count = static_cast<int>(m_items.size());
    m_selected = static_cast<size_t>(utils::Wrap(static_cast<int>(m_selected) + direction, count));
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
        m_items[i].text->SetColor(i == m_selected ? colors::highlight : colors::menuItem); // choose correct color
    }
}
