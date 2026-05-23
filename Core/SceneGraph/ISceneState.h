#pragma once

namespace bengine {
    class ISceneState {
    public:
        virtual ~ISceneState() = default;

        virtual void OnEnter() {}
        virtual void OnExit() {}
        virtual void Update(float /*deltaTime*/) {}

    protected:
        ISceneState() = default;
    };
}
