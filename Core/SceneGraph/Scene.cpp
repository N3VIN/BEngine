#include <cassert>
#include "Scene.h"

using namespace bengine;


GameObject *Scene::Add(std::unique_ptr<GameObject> object) {
    assert(object != nullptr && "Cannot add a null GameObject to the scene.");
    return m_objects.emplace_back(std::move(object)).get();
}

void Scene::Remove(GameObject *object) {
    const auto it = std::ranges::find_if(
        m_objects, [object](const auto &go) {
            return go.get() == object;
        }
    );

    if (it == m_objects.end())
        return;

    while (object->GetChildCount() > 0) {
        Remove(object->GetChildAt(0)); // recursively delete all the children
    }

    object->SetParent(nullptr);
    m_markedObjects.emplace_back(std::move(*it));
}

void Scene::RemoveAll() {
    m_objects.clear();
}

void Scene::Update(float deltaTime) {
    for (auto &object: m_objects) {
        if (object.get() != nullptr) {
            object->Update(deltaTime);
        }
    }

    DeleteMarkedObjects();
}

void Scene::FixedUpdate() {
    for (auto &object: m_objects) {
        if (object.get() != nullptr) {
            object->FixedUpdate();
        }
    }
}

void Scene::Render() const {
    for (const auto &object: m_objects) {
        if (object.get() != nullptr) {
            object->Render();
        }
    }
}

void Scene::DeleteMarkedObjects() {
    std::erase(m_objects, nullptr);
    m_markedObjects.clear();
}
