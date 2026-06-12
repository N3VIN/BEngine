# BEngine

A small 2D engine built on SDL3. Code lives in `Core/` (namespace `bengine`), every game target links it as a static lib. This README is about the engine and the choices behind it.

I built NES Bomberman on top of it as a test bed.

## Game Loop

`Core/Engine/Engine.cpp`, `RunOneFrame()`. Fixed-timestep with a lag accumulator:

- measure real delta, add to `m_lag`
- drain `m_lag` in a `while`, each step is one `FixedUpdate()` at fixed dt
- one variable `Update(deltaTime)`, then `Render()`

Deterministic gameplay goes in `FixedUpdate`, frame-rate dependent stuff in `Update`. Native builds sleep to cap the frame, the web build doesn't because the browser drives the loop through `requestAnimationFrame`. Same `RunOneFrame` for both.

## Scene Graph and Components

A `Scene` owns its `GameObject`s by `unique_ptr`. Parent/child links are raw non-owning pointers, ownership already lives in the scene so theres no reason to reference-count the hierarchy too.

Behaviour is components. `AddComponent<T>(...)` constructs in place, `GetComponent<T>()` does a `dynamic_cast`. Both constrained with a `ComponentType` concept (`std::derived_from<T, Component>`) for readable errors at the call site.

The `Transform` is **not** a component, its just a plain member. I followed Unity here, since every object always has one. World transforms are cached and recomputed lazily, with a dirty flag that propagates down to the children.

## Patterns

- **Game Loop / Update** - the fixed timestep above.
- **Command** - input. `InputManager` binds scancode / gamepad button + key-state to an `ICommand`, so rebinds and the keyboard/gamepad split are just different bindings.
- **Service Locator** - hands out the audio service and the event bus. Audio has a `NullAudioService` fallback so unregistered calls no-op instead of crashing. It is also used for the web build.
- **Singleton** - long lived managers only (Renderer, SceneManager, ResourceManager, InputManager, Time). The course challenges singletons so I kept them to things that genuinely are global and single.
- **Observer** - `Subject` / `IObserver`, synchronous, observers register themselves.

## Multicast Delegate

I have the classic `Subject` / `IObserver` in the engine too, but honestly, after working with C# events and Unreal's `TMulticastDelegate`, the OOP-style observer pattern never really felt right to me. So taking some inspiration from the side notes in the Game Programming Patterns book, I implemented a typed `MulticastDelegate<Args...>` (`Core/Patterns/MulticastDelegate.h`) instead, where you just subscribe a callback and don't have to inherit from anything.

Then I added RAII unsubscription on top:

- `Subscribe` returns a `ScopedDelegate` that unsubscribes in its destructor, so a listener can't outlive its subscription.
- re-entrancy safe: unsubscribing during a `Broadcast` only flags the listener dead, the erase is deferred until the broadcast finishes, so you can't invalidate the vector mid-iteration.

## Event Bus

The delegate made my life a lot easier, but it scaled badly. To subscribe you need a pointer to the object that owns the delegate, so systems that had nothing to do with each other ended up holding references just to wire up a subscription.

So I added an `EventBus` (`Core/Patterns/EventBus.h`) on top of the delegate, not replacing it. It's a type-erased map of `std::type_index` → a `MulticastDelegate<const Event&>` slot. You `Subscribe<SomeEvent>(...)` / `Broadcast(SomeEvent{...})`, and the two sides only share the event *type*, never each other. The delegate still does the real work (RAII handle, re-entrancy safety), the bus is just a typed routing table over it, so the localized one-to-one relations still use a plain delegate.

The bus lives in the `ServiceLocator` instead of being its own singleton, a single known access point without creating another singleton.


`Tools/LevelPack` is a tiny offline tool that takes the JSON levels and packs each one into a binary `.bomb` file (a small header + the raw grid), so the game loads them straight from binary instead of parsing JSON at runtime. It also loads the JSON directly if it fails to load the `.bomb`. 

## Build

C++23, MSVC, `/W4 /WX` (warnings as errors). Templates constrained with concepts. `Data/` is copied next to the exe by a CMake post-build step with the SDL dll's.

### Windows

    cmake --preset x64-debug
    cmake --build out/build/x64-debug


### Emscripten (web)

Toolchain via [Chocolatey](https://chocolatey.org/) on Windows: `choco install -y cmake emscripten ninja python`. Then:

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja
    python -m http.server

and browse to http://localhost:8000.
