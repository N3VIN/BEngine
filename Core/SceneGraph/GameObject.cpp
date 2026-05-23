#include "SceneGraph/GameObject.h"
#include "Components/RenderComponent.h"


bengine::GameObject::~GameObject() = default;

void bengine::GameObject::Update(float deltaTime) {
    for (auto &component: m_components) {
        component->Update(deltaTime);
    }
}

void bengine::GameObject::FixedUpdate() {
    for (auto &component: m_components) {
        component->FixedUpdate();
    }
}

void bengine::GameObject::Render() const {
    for (const auto &component: m_components) {
        component->Render();
    }
}

void bengine::GameObject::SetLocalPosition(const glm::vec2 &position) {
    m_transform.SetLocalPosition(glm::vec3(position, 0.f));
    SetTransformDirty();
}

void bengine::GameObject::SetLocalRotation(const float radians) {
    m_transform.SetLocalRotation(radians);
    SetTransformDirty();
}

void bengine::GameObject::SetLocalScale(const glm::vec2 &scale) {
    m_transform.SetLocalScale(scale);
    SetTransformDirty();
}

const glm::vec3 &bengine::GameObject::GetLocalPosition() const {
    return m_transform.GetLocalPosition();
}

float bengine::GameObject::GetLocalRotation() const {
    return m_transform.GetLocalRotation();
}

const glm::vec2 &bengine::GameObject::GetLocalScale() const {
    return m_transform.GetLocalScale();
}

glm::vec2 bengine::GameObject::GetWorldPosition() const {
    UpdateWorldTransform();
    return m_transform.GetWorldPosition();
}

float bengine::GameObject::GetWorldRotation() const {
    UpdateWorldTransform();
    return m_transform.GetWorldRotation();
}

glm::vec2 bengine::GameObject::GetWorldScale() const {
    return m_transform.GetWorldScale();
}

const glm::mat3 &bengine::GameObject::GetWorldTransform() const {
    UpdateWorldTransform();
    return m_transform.GetWorldTransform();
}

void bengine::GameObject::SetTransformDirty() const {
    m_transform.SetDirty();
    for (auto *child: m_children) {
        child->SetTransformDirty();
    }
}

void bengine::GameObject::RemoveAndSetNewParent(GameObject *parent) {
    if (m_parent) {
        m_parent->RemoveChild(this);
    }
    if (parent) {
        parent->AddChild(this);
    }
    else {
        m_parent = nullptr;
    }
}

void bengine::GameObject::UpdateWorldTransform() const {
    if (m_transform.IsDirty()) {
        if (m_parent) {
            m_transform.UpdateWorldTransform(m_parent->GetWorldTransform());
        }
        else {
            m_transform.UpdateWorldTransform();
        }
    }
}

void bengine::GameObject::SetParent(GameObject *parent, bool keepWorldPosition) {
    if (parent == this || m_parent == parent)
        return;

    if (keepWorldPosition) {
        const auto currentWorld = GetWorldTransform(); // save the old transform to apply it later
        RemoveAndSetNewParent(parent);

        if (parent) {
            const auto parentWorld = parent->GetWorldTransform();
            const auto newLocal = glm::inverse(parentWorld) * currentWorld;
            m_transform.SetLocalFromMatrix(newLocal);
        }
        else {
            m_transform.SetLocalFromMatrix(currentWorld);
        }

        SetTransformDirty();
    }
    else {
        RemoveAndSetNewParent(parent);
        SetTransformDirty();
    }
}

bengine::GameObject *bengine::GameObject::GetParent() const {
    return m_parent;
}

size_t bengine::GameObject::GetChildCount() const {
    return m_children.size();
}

bengine::GameObject *bengine::GameObject::GetChildAt(size_t index) const {
    return m_children[index];
}

void bengine::GameObject::AddChild(GameObject *child) {
    if (!child || child == this) {
        return;
    }

    if (child->m_parent == this) {
        return;
    }

    if (child->m_parent) {
        child->m_parent->RemoveChild(child);
    }

    child->m_parent = this; // setting the parent
    m_children.emplace_back(child);
}

void bengine::GameObject::RemoveChild(GameObject *child) {
    if (!child)
        return;

    const auto it = std::ranges::find(m_children, child);
    if (it == m_children.end())
        return;

    m_children.erase(it);
    child->m_parent = nullptr;
}

bool bengine::GameObject::IsChild(const GameObject *object) const {
    if (object == nullptr)
        return false;

    for (const auto *child: m_children) {
        if (child == object || child->IsChild(object))
            return true;
    }
    return false;
}
