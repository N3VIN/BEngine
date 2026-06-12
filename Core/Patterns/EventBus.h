#pragma once
#include <concepts>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "MulticastDelegate.h"

namespace bengine {
    template<typename Event>
    concept EventType = std::is_object_v<Event>;

    // loosely inspired by the immediate event bus https://docs.vulkan.org/tutorial/latest/Building_a_Simple_Engine/Engine_Architecture/06_event_systems.html
    // and adapted to use our existing MulticastDelegate with a hashmap instead of a vector
    class EventBus final {
    public:
        EventBus() = default;
        ~EventBus() = default;

        EventBus(const EventBus &) = delete;
        EventBus &operator=(const EventBus &) = delete;
        EventBus(EventBus &&) = delete;
        EventBus &operator=(EventBus &&) = delete;

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<EventType Event, typename Fn>
            requires std::invocable<Fn, const Event &>
        [[nodiscard]] ScopedDelegate Subscribe(Fn &&callback) {
            return GetEventSlot<Event>().delegate.Subscribe(std::forward<Fn>(callback));
        }

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<EventType Event, typename Class>
        [[nodiscard]] ScopedDelegate Subscribe(Class *instance, void (Class::*memberFunction)(const Event &)) {
            return GetEventSlot<Event>().delegate.Subscribe(instance, memberFunction);
        }

        // IMPORTANT, dont call Subscribe during Broadcast, only in constuctors or during init logic
        template<EventType Event, typename Class>
        [[nodiscard]] ScopedDelegate Subscribe(Class *instance, void (Class::*memberFunction)(const Event &) const) {
            return GetEventSlot<Event>().delegate.Subscribe(instance, memberFunction);
        }

        template<EventType Event>
        void Broadcast(const Event &event) {
            const auto key = std::type_index(typeid(Event));
            const auto it = m_eventSlots.find(key);
            if (it == m_eventSlots.end()) {
                return;
            }

            auto slot = static_cast<EventSlot<Event> *>(it->second.get());
            slot->delegate.Broadcast(event);
        }

        template<EventType Event>
        [[nodiscard]] bool HasSubscribers() const {
            const auto key = std::type_index(typeid(Event));
            const auto it = m_eventSlots.find(key);
            if (it == m_eventSlots.end()) {
                return false;
            }

            auto *slot = static_cast<EventSlot<Event> *>(it->second.get());
            return !slot->delegate.IsListenersEmpty();
        }

    private:
        struct EventSlotBase { // for type erasure to store in the hash map
            virtual ~EventSlotBase() = default;
        };

        template<EventType Event>
        struct EventSlot final : EventSlotBase {
            MulticastDelegate<const Event &> delegate{};
        };

        template<EventType Event>
        EventSlot<Event> &GetEventSlot() {
            const auto key = std::type_index(typeid(Event));
            auto &slot = m_eventSlots[key];
            if (!slot) {
                slot = std::make_unique<EventSlot<Event> >();
            }

            return *static_cast<EventSlot<Event> *>(slot.get());
        }

        std::unordered_map<std::type_index, std::unique_ptr<EventSlotBase> > m_eventSlots{};
    };
}
