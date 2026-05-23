#pragma once

namespace bengine {
    class GameObject;

    class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void Execute() = 0;
    };

    class GameObjectCommand : public ICommand {
    public:
        explicit GameObjectCommand(GameObject *gameObject)
            : m_GameObject(gameObject) {}

    protected:
        [[nodiscard]] GameObject *GetGameObject() const {
            return m_GameObject;
        }

    private:
        GameObject *m_GameObject;
    };
}
