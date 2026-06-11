#include <sstream>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif
#include "Engine.h"

#include <thread>

#include "Input/InputManager.h"
#include "SceneGraph/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/ResourceManager.h"
#include "TimeManager.h"

SDL_Window *g_window{};

void LogSDLVersion(const std::string &message, int major, int minor, int patch) {
#if WIN32
    std::stringstream ss;
    ss << message << major << "." << minor << "." << patch << "\n";
    OutputDebugString(ss.str().c_str());
#else
    std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void *arg) {
    static_cast<bengine::Engine *>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion() {
    LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    int version = SDL_GetVersion();
    LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
    // LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
    // version = IMG_Version();
    // LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
    LogSDLVersion("Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
    version = TTF_Version();
    LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
}

bengine::Engine::Engine(const fs::path &dataPath) {
#if USE_STEAMWORKS
    if (!SteamAPI_Init())
        throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
#endif

    PrintSDLVersion();

    std::cout << "Loading engine data..." << std::endl;

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("Renderer error: %s", SDL_GetError());
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(
        "Bomberman",
        768,
        696,
        SDL_WINDOW_OPENGL
    );
    if (g_window == nullptr) {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    Renderer::GetInstance().Init(g_window);
    ResourceManager::GetInstance().Init(dataPath);
}

bengine::Engine::~Engine() {
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();

#if USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
}

void bengine::Engine::Run(const std::function<void()> &load) {
    load();

    m_lastTime = std::chrono::high_resolution_clock::now();
    m_lag = 0.0f;

#ifndef __EMSCRIPTEN__
    while (!m_quit) {
        RunOneFrame();
    }
#else
    emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void bengine::Engine::RunOneFrame() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    Time::GetInstance().deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
    m_lastTime = currentTime;
    m_lag += Time::GetInstance().deltaTime;

#if USE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif

    m_quit = !InputManager::GetInstance().ProcessInput();

    while (m_lag >= Time::GetInstance().fixedDeltaTime) {
        SceneManager::GetInstance().FixedUpdate();
        m_lag -= Time::GetInstance().fixedDeltaTime;
    }

    SceneManager::GetInstance().Update(Time::GetInstance().deltaTime);
    Renderer::GetInstance().Render();

    // sleep only for the native build, browser handles the RequestAnimationFrame
#ifndef __EMSCRIPTEN__
    const auto sleepTime = currentTime + std::chrono::milliseconds(Time::GetInstance().MS_PER_FRAME) - std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(sleepTime);
#endif
}
