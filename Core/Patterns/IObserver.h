#pragma once

namespace bengine {
    class GameObject;

    enum class GameEvent {
        PlayerDied,
        PickupPicked
    };

    class IObserver {
    public:
        // IMPORTANT: child classes must implement a destructor that calls RemoveObserver(this) on its subject or it will result in dangling pointer in the subject!!
        virtual ~IObserver() = default;
        virtual void OnNotify(GameObject *gameObject, GameEvent event) = 0;
    };
}
