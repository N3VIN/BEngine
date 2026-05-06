#pragma once
#include <functional>
#include <memory>
#include <utility>

#include "../../Core/Patterns/ICommand.h"
#include "../../Core/Patterns/ISceneState.h"
#include "../../Core/SceneGraph/SceneManager.h"

namespace dae {
    class ChangeSceneCommand final : public ICommand {
    public:
        using Factory = std::function<std::unique_ptr<ISceneState>()>;

        explicit ChangeSceneCommand(Factory factory)
            : m_factory(std::move(factory)) {}

        void Execute() override {
            SceneManager::GetInstance().SetState(m_factory());
        }

    private:
        Factory m_factory;
    };
}
