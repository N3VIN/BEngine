#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Engine/Engine.h"
#include "SceneGraph/SceneManager.h"
#include "Patterns/ServiceLocator.h"
#include "Audio/SDLAudioService.h"
#include "Audio/NullAudioService.h"

#include "HighScores.h"
#include "SceneStates/MainMenuState.h"

namespace fs = std::filesystem;

static void load() {
#if __EMSCRIPTEN__
    bengine::ServiceLocator::RegisterAudioService(std::make_unique<bengine::NullAudioService>());
#else
    bengine::ServiceLocator::RegisterAudioService(std::make_unique<bengine::SDLAudioService>());
#endif
    bomberman::highscores::Load();
    bengine::SceneManager::GetInstance().SetState(std::make_unique<bomberman::MainMenuState>());
}

int main(int, char *[]) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    bengine::Engine engine(data_location);
    engine.Run(load);
    return 0;
}
