#pragma once
#include <concepts>
#include <utility>
#include "Components/Component.h"
#include "Patterns/MulticastDelegate.h"
#include "Timer.h"

namespace bomberman {
    class HealthComponent final : public bengine::Component {
    public:
        HealthComponent(bengine::GameObject *parent, int lives);
        ~HealthComponent() override = default;

        void Update(float deltaTime) override;
        void TakeDamage(int amount, bengine::GameObject *attacker = nullptr);

        [[nodiscard]] int GetLives() const;
        [[nodiscard]] bool IsAlive() const;
        [[nodiscard]] bengine::GameObject *GetLastAttacker() const;

        template<typename Fn>
            requires std::invocable<Fn, int>
        [[nodiscard]] bengine::ScopedDelegate SubscribeDamaged(Fn &&callback) {
            return m_onDamaged.Subscribe(std::forward<Fn>(callback));
        }

    private:
        int m_lives;
        bengine::GameObject *m_lastAttacker{nullptr};
        bengine::Timer m_iframes{};
        bengine::MulticastDelegate<int> m_onDamaged{};
    };
}
