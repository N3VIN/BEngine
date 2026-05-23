#pragma once

namespace bengine {
    class GameObject;

    class Component {
    public:
        virtual ~Component() = default;
        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
        Component(Component &&) = delete;
        Component &operator=(Component &&) = delete;

        virtual void Update(float /*deltaTime*/) {}
        virtual void FixedUpdate() {}
        virtual void Render() const {}

    protected:
        explicit Component(GameObject *parent)
            : m_parent(parent) {}

        [[nodiscard]] GameObject *GetParent() const {
            return m_parent;
        }

    private:
        GameObject *m_parent = nullptr;
    };
} // bengine
