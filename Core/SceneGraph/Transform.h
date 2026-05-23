#pragma once
#include <glm/glm.hpp>

namespace bengine {
    class Transform final {
    public:
        void SetLocalPosition(const glm::vec3 &position);
        void SetLocalRotation(float radians);
        void SetLocalScale(const glm::vec2 &scale);
        void SetLocalFromMatrix(const glm::mat3 &matrix);
        [[nodiscard]] const glm::vec3 &GetLocalPosition() const;
        [[nodiscard]] float GetLocalRotation() const;
        [[nodiscard]] const glm::vec2 &GetLocalScale() const;
        void SetDirty();
        [[nodiscard]] bool IsDirty() const;
        void UpdateWorldTransform(const glm::mat3 &parentWorld);
        void UpdateWorldTransform();
        [[nodiscard]] const glm::mat3 &GetWorldTransform() const;
        [[nodiscard]] glm::vec2 GetWorldPosition() const;
        [[nodiscard]] float GetWorldRotation() const;
        [[nodiscard]] glm::vec2 GetWorldScale() const;

    private:
        [[nodiscard]] glm::mat3 ComputeLocalTransform() const;

        glm::vec3 m_localPosition{};
        float m_localRotation{0.f};
        glm::vec2 m_localScale{1.f, 1.f};

        glm::mat3 m_worldTransform{1.f};
        bool m_isDirty{true};
    };
}
