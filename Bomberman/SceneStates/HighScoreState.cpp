#include "HighScoreState.h"

#include "MainMenuState.h"
#include "MenuUI.h"
#include "ScoreBoard.h"
#include "HighScores.h"
#include "Components/NameEntryComponent.h"
#include "Commands/ChangeSceneCommand.h"
#include "Commands/NameEntryNavigateCommand.h"
#include "Commands/NameEntryConfirmCommand.h"

#include <algorithm>
#include <string>

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "UIHelpers.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"

namespace {
    glm::vec2 Centered(float fractionY) {
        return bengine::ScreenFraction(0.5f, fractionY);
    }
}

bomberman::HighScoreState::HighScoreState(std::vector<NewScore> newScores)
    : m_newScores(std::move(newScores)) {}

bomberman::HighScoreState::~HighScoreState() {
    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
    }
}

void bomberman::HighScoreState::OnEnter() {
    bengine::Renderer::GetInstance().SetActiveCamera(nullptr);
    Rebuild();
}

void bomberman::HighScoreState::OnExit() {
    bengine::InputManager::GetInstance().UnbindAll();
}

void bomberman::HighScoreState::Update(float) {
    if (m_dirty) {
        m_dirty = false;
        Rebuild();
    }
}

void bomberman::HighScoreState::Rebuild() {
    bengine::InputManager::GetInstance().UnbindAll();

    if (m_scene) {
        bengine::SceneManager::GetInstance().DestroyScene(*m_scene);
        m_scene = nullptr;
    }

    m_nameEntry = nullptr;
    m_scene = &bengine::SceneManager::GetInstance().CreateScene();

    if (m_newScores.empty()) {
        BuildTable();
    }
    else {
        BuildEntry(m_newScores.front());
    }

    bengine::SceneManager::GetInstance().SetActiveScene(*m_scene);
}

void bomberman::HighScoreState::BuildTable() const {
    CreateMenuLabel(*m_scene, "HIGH SCORES", 48, {255, 255, 255, 255}, Centered(0.15f));

    const auto &table = highscores::Table();
    if (table.empty()) {
        CreateMenuLabel(*m_scene, "NO SCORES YET", 28, {180, 180, 180, 255}, Centered(0.45f));
    }
    else {
        const auto padLeft = [](std::string value, size_t width) {
            if (value.size() < width) {
                value.insert(0, width - value.size(), ' ');
            }
            return value;
        };

        size_t scoreWidth = 1;
        for (const auto &record: table) {
            scoreWidth = std::max(scoreWidth, std::to_string(record.score).size());
        }

        constexpr float startY = 0.3f;
        constexpr float stepY = 0.06f;
        for (size_t rank = 0; rank < table.size(); ++rank) {
            const std::string name(table[rank].name.begin(), table[rank].name.end());
            const std::string line = padLeft(std::to_string(rank + 1), 2) + "  " + name + "  " + padLeft(std::to_string(table[rank].score), scoreWidth);
            CreateMenuLabel(*m_scene, line, 24, {220, 220, 220, 255}, Centered(startY + static_cast<float>(rank) * stepY));
        }
    }

    CreateMenuLabel(*m_scene, "PRESS ESC", 24, {160, 160, 160, 255}, Centered(0.85f));

    BindTable();
}

void bomberman::HighScoreState::BuildEntry(const NewScore &entry) {
    CreateMenuLabel(*m_scene, "NEW HIGH SCORE", 40, {255, 255, 255, 255}, Centered(0.18f));

    if (!entry.label.empty()) {
        CreateMenuLabel(*m_scene, entry.label, 28, {255, 255, 80, 255}, Centered(0.3f));
    }

    CreateMenuLabel(*m_scene, "SCORE  " + std::to_string(entry.score), 28, {255, 255, 255, 255}, Centered(0.37f));

    auto *entryGo = m_scene->Add(std::make_unique<bengine::GameObject>());
    m_nameEntry = entryGo->AddComponent<NameEntryComponent>();

    constexpr float slotStartX = 0.42f;
    constexpr float slotStepX = 0.08f;
    for (int slot = 0; slot < highscores::NameLength; ++slot) {
        auto *slotLabel = CreateMenuLabel(*m_scene, "A", 48, {180, 180, 180, 255}, bengine::ScreenFraction(slotStartX + static_cast<float>(slot) * slotStepX, 0.5f));
        m_nameEntry->AddSlot(slotLabel);
    }

    m_nameEntry->SetOnConfirm([this, score = entry.score](std::array<char, highscores::NameLength> name) {
            highscores::Insert(name, score);
            if (!m_newScores.empty()) {
                m_newScores.erase(m_newScores.begin());
            }

            m_dirty = true; // deferred to update
        }
    );

    CreateMenuLabel(*m_scene, "W S LETTER  A D SLOT  ENTER OK", 16, {160, 160, 160, 255}, Centered(0.7f));

    BindEntry();
}

void bomberman::HighScoreState::BindEntry() const {
    auto &input = bengine::InputManager::GetInstance();

    const auto navigate = [this](int letterDir, int slotDir) {
        return std::make_unique<NameEntryNavigateCommand>(m_nameEntry, letterDir, slotDir);
    };

    input.BindCommand(SDL_SCANCODE_W, bengine::KeyState::Down, navigate(1, 0));
    input.BindCommand(SDL_SCANCODE_S, bengine::KeyState::Down, navigate(-1, 0));
    input.BindCommand(SDL_SCANCODE_A, bengine::KeyState::Down, navigate(0, -1));
    input.BindCommand(SDL_SCANCODE_D, bengine::KeyState::Down, navigate(0, 1));
    input.BindCommand(0, bengine::Gamepad::Button::DpadUp, bengine::KeyState::Down, navigate(1, 0));
    input.BindCommand(0, bengine::Gamepad::Button::DpadDown, bengine::KeyState::Down, navigate(-1, 0));
    input.BindCommand(0, bengine::Gamepad::Button::DpadLeft, bengine::KeyState::Down, navigate(0, -1));
    input.BindCommand(0, bengine::Gamepad::Button::DpadRight, bengine::KeyState::Down, navigate(0, 1));

    const auto confirm = [this] {
        return std::make_unique<NameEntryConfirmCommand>(m_nameEntry);
    };

    input.BindCommand(SDL_SCANCODE_RETURN, bengine::KeyState::Down, confirm());
    input.BindCommand(SDL_SCANCODE_SPACE, bengine::KeyState::Down, confirm());
    input.BindCommand(0, bengine::Gamepad::Button::A, bengine::KeyState::Down, confirm());
    input.BindCommand(0, bengine::Gamepad::Button::Start, bengine::KeyState::Down, confirm());
}

void bomberman::HighScoreState::BindTable() const {
    auto &input = bengine::InputManager::GetInstance();

    const auto toMenu = [] {
        return std::make_unique<ChangeSceneCommand>([] {
                return std::make_unique<MainMenuState>();
            }
        );
    };

    input.BindCommand(SDL_SCANCODE_ESCAPE, bengine::KeyState::Down, toMenu());
    input.BindCommand(0, bengine::Gamepad::Button::B, bengine::KeyState::Down, toMenu());
    input.BindCommand(0, bengine::Gamepad::Button::Back, bengine::KeyState::Down, toMenu());
}

std::vector<bomberman::HighScoreState::NewScore> bomberman::GetNewScores() {
    std::vector<HighScoreState::NewScore> newScores{};

    const int playerCount = scoreboard::PlayerCount();
    for (int player = 0; player < playerCount; ++player) {
        const int score = scoreboard::Get(player);
        if (highscores::Qualifies(score)) {
            const std::string label = (playerCount > 1) ? "P" + std::to_string(player + 1) : "";
            newScores.push_back({label, score});
        }
    }

    return newScores;
}
