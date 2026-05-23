#pragma once
#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

namespace bengine {
    inline constexpr uint64_t INVALID_ID = 0;

    class DelegateHandle final {
    public:
        constexpr DelegateHandle() noexcept = default;
        constexpr bool operator==(const DelegateHandle &other) const noexcept = default;

        [[nodiscard]] constexpr bool IsValid() const noexcept {
            return m_id != INVALID_ID;
        }

    private:
        uint64_t m_id{INVALID_ID};

        template<typename...>
        friend class MulticastDelegate;

        constexpr explicit DelegateHandle(uint64_t id) noexcept
            : m_id(id) {}
    };

    // RAII Unsubscription
    class ScopedDelegate final {
    public:
        constexpr ScopedDelegate() noexcept = default;

        explicit ScopedDelegate(DelegateHandle handle, std::function<void()> cleanup)
            : m_handle(handle)
          , m_cleanup(std::move(cleanup)) {}

        ~ScopedDelegate() {
            if (m_cleanup) {
                m_cleanup();
            }
        }

        ScopedDelegate(const ScopedDelegate &) = delete;
        ScopedDelegate &operator=(const ScopedDelegate &) = delete;
        ScopedDelegate(ScopedDelegate &&other) noexcept = default;
        ScopedDelegate &operator=(ScopedDelegate &&other) noexcept = default;

        [[nodiscard]] DelegateHandle GetHandle() const noexcept {
            return m_handle;
        }

    private:
        DelegateHandle m_handle; // can be used for manual unsubscription
        std::function<void()> m_cleanup;
    };

    template<typename... Args>
    class MulticastDelegate final {
    public:
        MulticastDelegate() = default;
        ~MulticastDelegate() = default;

        MulticastDelegate(const MulticastDelegate &) = delete;
        MulticastDelegate &operator=(const MulticastDelegate &) = delete;
        MulticastDelegate(MulticastDelegate &&) = delete;
        MulticastDelegate &operator=(MulticastDelegate &&) = delete;

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<typename Fn>
        [[nodiscard]] ScopedDelegate Subscribe(Fn &&callback) {
            const DelegateHandle handle{++m_handleID};
            m_listeners.push_back(ListenerHandle{handle, std::forward<Fn>(callback), true});
            return ScopedDelegate{
                handle,
                [this, handle]() {
                    Unsubscribe(handle);
                }
            };
        }

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<typename Class>
        [[nodiscard]] ScopedDelegate Subscribe(Class *instance, void (Class::*memberFunction)(Args...)) {
            return Subscribe([instance, memberFunction](Args... args) {
                    std::invoke(memberFunction, instance, args...);
                }
            );
        }

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<typename Class>
        [[nodiscard]] ScopedDelegate Subscribe(Class *instance, void (Class::*memberFunction)(Args...) const) { // const member function
            return Subscribe([instance, memberFunction](Args... args) {
                    std::invoke(memberFunction, instance, args...);
                }
            );
        }

        bool Unsubscribe(DelegateHandle handle) {
            if (!handle.IsValid()) {
                return false;
            }

            bool foundListener = false;
            for (auto &listener: m_listeners) {
                if (listener.handle == handle && listener.isAlive) {
                    listener.isAlive = false;
                    foundListener = true;
                    break;
                }
            }

            if (!foundListener) {
                return false;
            }

            if (m_depth == 0) {
                std::erase_if(m_listeners, [](ListenerHandle &lh) {
                                  return !lh.isAlive;
                              }
                );
            }
            else {
                m_isDirty = true; // mark for delete
            }

            return true;
        }

        void Broadcast(Args... args) {
            ++m_depth;
            for (const auto &listener: m_listeners) {
                if (!listener.isAlive) {
                    continue;
                }

                std::invoke(listener.callbackFn, args...); // call the subscribed function
            }

            --m_depth;
            // defer until all the broadcast finishes and only when unsubscribe is called
            // we do this to enable early unsubscription which is optional
            if (m_depth == 0 && m_isDirty) {
                std::erase_if(m_listeners, [](ListenerHandle &handle) {
                                  return !handle.isAlive;
                              }
                );

                m_isDirty = false;
            }
        }

        [[nodiscard]] bool IsListenersEmpty() const noexcept {
            return std::ranges::none_of(m_listeners, &ListenerHandle::isAlive);
        }

    private:
        struct ListenerHandle final {
            DelegateHandle handle;
            std::function<void(Args...)> callbackFn;
            bool isAlive{};
        };

        uint64_t m_handleID{INVALID_ID};
        std::vector<ListenerHandle> m_listeners{};
        int m_depth{0};
        bool m_isDirty{false};
    };
}
