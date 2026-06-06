#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

namespace bengine {
    class Scene final {
    public:
        GameObject *Add(std::unique_ptr<GameObject> object);
        void Remove(GameObject *object);
        void RemoveAll();

        void Update(float deltaTime);
        void FixedUpdate();
        void Render() const;

        ~Scene() = default;
        Scene(const Scene &other) = delete;
        Scene(Scene &&other) = delete;
        Scene &operator=(const Scene &other) = delete;
        Scene &operator=(Scene &&other) = delete;

    private:
        friend class SceneManager;
        explicit Scene() = default;

        void DeleteMarkedObjects();

        std::vector<std::unique_ptr<GameObject> > m_objects{};
        std::vector<std::unique_ptr<GameObject> > m_markedObjects{};
    };
}
