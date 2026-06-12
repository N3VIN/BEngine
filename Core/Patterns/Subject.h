#pragma once
#include <vector>

#include "IObserver.h"

namespace bengine {
    class Subject {
    public:
        virtual ~Subject() = default;

        void AddObserver(IObserver *observer) {
            m_observers.push_back(observer);
        }

        void RemoveObserver(IObserver *observer) {
            std::erase(m_observers, observer);
        }

    protected:
        void Notify(GameObject *gameObject, GameEvent event) const {
            for (auto observer: m_observers) {
                observer->OnNotify(gameObject, event);
            }
        }

    private:
        std::vector<IObserver *> m_observers;
    };
} // bengine
