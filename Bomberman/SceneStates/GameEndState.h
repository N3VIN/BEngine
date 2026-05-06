#pragma once
#include "../../Core/Patterns/ISceneState.h"

namespace dae {
    class Scene;

    class GameEndState final : public ISceneState {
    public:
        GameEndState();
        ~GameEndState() override;

        void OnEnter() override;
        void OnExit() override;

    private:
        Scene *m_scene{};
    };
}
