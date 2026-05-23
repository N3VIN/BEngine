#pragma once

namespace bengine {
    class Timer final {
    public:
        Timer() = default;

        explicit Timer(float duration)
            : m_duration(duration)
          , m_remaining(duration) {}

        void Update(float deltaTime) {
            if (m_remaining <= 0.f) {
                return;
            }

            m_remaining -= deltaTime;
            if (m_remaining < 0.f) {
                m_remaining = 0.f;
            }
        }

        [[nodiscard]] bool IsExpired() const {
            return m_remaining <= 0.f;
        }

        void Reset() {
            m_remaining = m_duration;
        }

        void SetDuration(float duration) {
            m_duration = duration;
            m_remaining = duration;
        }

        [[nodiscard]] float GetRemaining() const {
            return m_remaining;
        }

        [[nodiscard]] float GetProgress() const {
            if (m_duration <= 0.f) {
                return 1.f;
            }

            return 1.f - m_remaining / m_duration;
        }

    private:
        float m_duration{};
        float m_remaining{};
    };
}
