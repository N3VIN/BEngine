#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "utils.h"
#include "Engine/Engine.h"
#include "SceneGraph/SceneManager.h"
#include "Renderer/ResourceManager.h"
#include "SceneGraph/Scene.h"
#include "Patterns/ServiceLocator.h"
#include "Audio/SDLAudioService.h"
#include "GameEvents.h"

#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/HealthComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/PickupComponent.h"
#include "Components/ScoreDisplayComponent.h"
#include "Components/SteamAchievementComponent.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"
#include "Commands/MoveCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/ScoreCommand.h"

namespace fs = std::filesystem;

static void load() {
    auto &scene = dae::SceneManager::GetInstance().CreateScene();

    auto &audio = dae::ServiceLocator::GetAudioService();
    audio.LoadAudio(utils::Hash("damage"), dae::ResourceManager::GetInstance().GetDataPath() / "damage.wav");

    auto go = std::make_unique<dae::GameObject>();
    auto *bg = go->AddComponent<dae::RenderComponent>();
    bg->SetTexture("background.png");
    scene.Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::RenderComponent>()->SetTexture("logo.png");
    go->SetLocalPosition(glm::vec2(358, 180));
    scene.Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto *textComp = go->AddComponent<dae::TextComponent>();
    textComp->SetFont(font);
    textComp->SetText("Programming 4 Assignment");
    textComp->SetColor({255, 255, 0, 255});
    go->SetLocalPosition(glm::vec2(292, 20));
    scene.Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    auto *fpsText = go->AddComponent<dae::TextComponent>();
    fpsText->SetFont(dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
    fpsText->SetColor({0, 255, 0, 255});
    go->AddComponent<dae::FPSComponent>();
    go->SetLocalPosition(glm::vec2(10, 10));
    scene.Add(std::move(go));

    //

    constexpr int startingLives = 3;
    const auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

    auto textGo1 = std::make_unique<dae::GameObject>();
    textGo1->SetLocalPosition(glm::vec2(10, 70));
    auto *Text1 = textGo1->AddComponent<dae::TextComponent>();
    Text1->SetFont(smallFont);
    Text1->SetColor({255, 255, 255, 255});
    Text1->SetText("Use the D-Pad to move Bomberman 1, X to inflict damage, A and B to pick up pellets");
    scene.Add(std::move(textGo1));

    auto textGo2 = std::make_unique<dae::GameObject>();
    textGo2->SetLocalPosition(glm::vec2(10, 95));
    auto *Text2 = textGo2->AddComponent<dae::TextComponent>();
    Text2->SetFont(smallFont);
    Text2->SetColor({255, 255, 255, 255});
    Text2->SetText("Use WASD to move Bomberman 2, C to inflict damage, Z and X to pick up pellets");
    scene.Add(std::move(textGo2));

    // player 1
    auto player1 = std::make_unique<dae::GameObject>();
    player1->AddComponent<dae::RenderComponent>()->SetTexture("bomberman.png");
    player1->SetLocalPosition(glm::vec2(300, 300));
    auto *p1Health = player1->AddComponent<dae::HealthComponent>(startingLives);

    auto *p1Pickup = player1->AddComponent<dae::PickupComponent>();
    auto *p1Achievement = player1->AddComponent<dae::SteamAchievementComponent>();
    auto *p1 = player1.get();
    scene.Add(std::move(player1));

    // player 1 lives display
    auto livesDisplay1 = std::make_unique<dae::GameObject>();
    livesDisplay1->SetLocalPosition(glm::vec2(10, 125));
    auto *livesText1 = livesDisplay1->AddComponent<dae::TextComponent>();
    livesText1->SetFont(smallFont);
    livesText1->SetColor({255, 255, 255, 255});
    livesDisplay1->AddComponent<dae::LivesDisplayComponent>(p1Health);
    // p1Health->AddObserver(livesComp1);
    scene.Add(std::move(livesDisplay1));

    // player 1 score display
    auto scoreDisplay1 = std::make_unique<dae::GameObject>();
    scoreDisplay1->SetLocalPosition(glm::vec2(10, 150));
    auto *scoreText1 = scoreDisplay1->AddComponent<dae::TextComponent>();
    scoreText1->SetFont(smallFont);
    scoreText1->SetColor({255, 255, 255, 255});
    auto *scoreComp1 = scoreDisplay1->AddComponent<dae::ScoreDisplayComponent>();
    p1Pickup->AddObserver(scoreComp1);
    p1Pickup->AddObserver(p1Achievement);
    scene.Add(std::move(scoreDisplay1));

    // player 2
    auto player2 = std::make_unique<dae::GameObject>();
    player2->AddComponent<dae::RenderComponent>()->SetTexture("bomberman.png");
    player2->SetLocalPosition(glm::vec2(500, 300));
    auto *p2Health = player2->AddComponent<dae::HealthComponent>(startingLives);

    auto *p2Pickup = player2->AddComponent<dae::PickupComponent>();
    auto *p2Achievement = player2->AddComponent<dae::SteamAchievementComponent>();
    auto *p2 = player2.get();
    scene.Add(std::move(player2));

    // player 2 lives display
    auto livesDisplay2 = std::make_unique<dae::GameObject>();
    livesDisplay2->SetLocalPosition(glm::vec2(10, 175));
    auto *livesText2 = livesDisplay2->AddComponent<dae::TextComponent>();
    livesText2->SetFont(smallFont);
    livesText2->SetColor({255, 200, 100, 255});
    livesDisplay2->AddComponent<dae::LivesDisplayComponent>(p2Health);
    // p2Health->AddObserver(livesComp2);
    scene.Add(std::move(livesDisplay2));

    // player 2 score display
    auto scoreDisplay2 = std::make_unique<dae::GameObject>();
    scoreDisplay2->SetLocalPosition(glm::vec2(10, 200));
    auto *scoreText2 = scoreDisplay2->AddComponent<dae::TextComponent>();
    scoreText2->SetFont(smallFont);
    scoreText2->SetColor({255, 200, 100, 255});
    auto *scoreComp2 = scoreDisplay2->AddComponent<dae::ScoreDisplayComponent>();
    p2Pickup->AddObserver(scoreComp2);
    p2Pickup->AddObserver(p2Achievement);
    scene.Add(std::move(scoreDisplay2));

    static auto damageSD = dae::ServiceLocator::GetEventBus().Subscribe<dae::events::PlayerDamaged>(
        [&audio](const dae::events::PlayerDamaged &) {
            audio.PlayAudio(utils::Hash("damage"), 1.0f);
        }
    );

    auto &input = dae::InputManager::GetInstance();
    constexpr float speed1 = 100.f;
    constexpr float speed2 = 200.f;
    constexpr int pickup1 = 10;
    constexpr int pickup2 = 100;

    // player 1 WASD, C to damage, Z and X for pickup
    input.BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p1, glm::vec2{0, -1}, speed1));
    input.BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p1, glm::vec2{0, 1}, speed1));
    input.BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p1, glm::vec2{-1, 0}, speed1));
    input.BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p1, glm::vec2{1, 0}, speed1));
    input.BindCommand(SDL_SCANCODE_C, dae::KeyState::Down, std::make_unique<dae::DamageCommand>(p1));
    input.BindCommand(SDL_SCANCODE_Z, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(p1, pickup1));
    input.BindCommand(SDL_SCANCODE_X, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(p1, pickup2));

    // player 2 DPad on controller 0, X to damage, A and B for pickup
    input.BindCommand(0, dae::Gamepad::Button::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p2, glm::vec2{0, -1}, speed2));
    input.BindCommand(0, dae::Gamepad::Button::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p2, glm::vec2{0, 1}, speed2));
    input.BindCommand(0, dae::Gamepad::Button::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p2, glm::vec2{-1, 0}, speed2));
    input.BindCommand(0, dae::Gamepad::Button::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(p2, glm::vec2{1, 0}, speed2));
    input.BindCommand(0, dae::Gamepad::Button::X, dae::KeyState::Down, std::make_unique<dae::DamageCommand>(p2));
    input.BindCommand(0, dae::Gamepad::Button::A, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(p2, pickup1));
    input.BindCommand(0, dae::Gamepad::Button::B, dae::KeyState::Down, std::make_unique<dae::ScoreCommand>(p2, pickup2));
}

int main(int, char *[]) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Engine engine(data_location);
    dae::ServiceLocator::RegisterAudioService(std::make_unique<dae::SDLAudioService>());
    engine.Run(load);
    return 0;
}
