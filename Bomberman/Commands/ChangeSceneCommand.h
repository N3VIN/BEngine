#pragma once
#include <functional>
#include <memory>
#include <utility>

#include "Patterns/ICommand.h"
#include "SceneGraph/ISceneState.h"
#include "SceneGraph/SceneManager.h"

namespace bomberman {
    class ChangeSceneCommand final : public bengine::ICommand {
    public:
        using Factory = std::function<std::unique_ptr<bengine::ISceneState>()>;

        explicit ChangeSceneCommand(Factory factory)
            : m_factory(std::move(factory)) {}

        void Execute() override {
            bengine::SceneManager::GetInstance().SetState(m_factory());
        }

    private:
        Factory m_factory;
    };
}
