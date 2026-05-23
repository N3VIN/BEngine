#include "Gamepad.h"


#if WIN32
// XInput
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>

class bengine::Gamepad::GamepadImpl {
public:
    explicit GamepadImpl(unsigned int controllerIndex)
        : m_controllerIndex(controllerIndex) {}

    void Update() {
        XINPUT_STATE state{};
        if (XInputGetState(m_controllerIndex, &state) == ERROR_SUCCESS) {
            const unsigned int prev = m_currentButtons;
            m_currentButtons = state.Gamepad.wButtons;
            const auto changes = m_currentButtons ^ prev;
            m_pressed = changes & m_currentButtons;
            m_released = changes & ~m_currentButtons;
        }
        else {
            m_pressed = m_released = m_currentButtons = 0;
        }
    }

    [[nodiscard]] unsigned int GetPressed(Button button) const {
        return m_pressed & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetReleased(Button button) const {
        return m_released & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetCurrent(Button button) const {
        return m_currentButtons & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetIndex() const {
        return m_controllerIndex;
    }

private:
    unsigned int m_controllerIndex;
    unsigned int m_pressed{};
    unsigned int m_released{};
    unsigned int m_currentButtons{};
};

#else
// SDL fallback
#include <SDL3/SDL.h>

class bengine::Gamepad::GamepadImpl {
public:
    explicit GamepadImpl(unsigned int controllerIndex)
        : m_controllerIndex(controllerIndex) {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);
    }

    ~GamepadImpl() {
        if (m_sdlGamepad) {
            SDL_CloseGamepad(m_sdlGamepad);
        }
    }

    void Update() {
        TryOpen();
        if (!m_sdlGamepad) {
            m_pressed = m_released = m_currentButtons = 0;
            return;
        }

        const unsigned int prev = m_currentButtons;
        m_currentButtons = BuildMask();
        const auto changes = m_currentButtons ^ prev;
        m_pressed = changes & m_currentButtons;
        m_released = changes & ~m_currentButtons;
    }

    [[nodiscard]] unsigned int GetPressed(Button button) const {
        return m_pressed & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetReleased(Button button) const {
        return m_released & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetCurrent(Button button) const {
        return m_currentButtons & static_cast<unsigned int>(button);
    }

    [[nodiscard]] unsigned int GetIndex() const {
        return m_controllerIndex;
    }

private:
    void TryOpen() {
        if (m_sdlGamepad) return;
        int count = 0;
        SDL_JoystickID *ids = SDL_GetGamepads(&count);
        if (ids && static_cast<int>(m_controllerIndex) < count) {
            m_sdlGamepad = SDL_OpenGamepad(ids[m_controllerIndex]);
        }

        SDL_free(ids);
    }

    unsigned int BuildMask() const {
        struct ButtonMapping {
            unsigned int mask;
            SDL_GamepadButton btn;
        };

        static constexpr std::array<ButtonMapping, 14> mappings{
            {
                {0x0001, SDL_GAMEPAD_BUTTON_DPAD_UP},
                {0x0002, SDL_GAMEPAD_BUTTON_DPAD_DOWN},
                {0x0004, SDL_GAMEPAD_BUTTON_DPAD_LEFT},
                {0x0008, SDL_GAMEPAD_BUTTON_DPAD_RIGHT},
                {0x0010, SDL_GAMEPAD_BUTTON_START},
                {0x0020, SDL_GAMEPAD_BUTTON_BACK},
                {0x0040, SDL_GAMEPAD_BUTTON_LEFT_STICK},
                {0x0080, SDL_GAMEPAD_BUTTON_RIGHT_STICK},
                {0x0100, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER},
                {0x0200, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER},
                {0x1000, SDL_GAMEPAD_BUTTON_SOUTH}, // A
                {0x2000, SDL_GAMEPAD_BUTTON_EAST},  // B
                {0x4000, SDL_GAMEPAD_BUTTON_WEST},  // X
                {0x8000, SDL_GAMEPAD_BUTTON_NORTH}, // Y
            }
        };

        unsigned int result = 0;
        for (const auto &m: mappings) {
            if (SDL_GetGamepadButton(m_sdlGamepad, m.btn)) {
                result |= m.mask;
            }
        }

        return result;
    }

    unsigned int m_controllerIndex;
    SDL_Gamepad *m_sdlGamepad{nullptr};
    unsigned int m_pressed{};
    unsigned int m_released{};
    unsigned int m_currentButtons{};
};

#endif

bengine::Gamepad::Gamepad(unsigned int controllerIndex)
    : m_pImpl(std::make_unique<GamepadImpl>(controllerIndex)) {}

bengine::Gamepad::~Gamepad() = default;
bengine::Gamepad::Gamepad(Gamepad &&) noexcept = default;
bengine::Gamepad &bengine::Gamepad::operator=(Gamepad &&) noexcept = default;

void bengine::Gamepad::Update() {
    m_pImpl->Update();
}

bool bengine::Gamepad::IsDown(Button button) const {
    return m_pImpl->GetPressed(button);
}

bool bengine::Gamepad::IsUp(Button button) const {
    return m_pImpl->GetReleased(button);
}

bool bengine::Gamepad::IsPressed(Button button) const {
    return m_pImpl->GetCurrent(button);
}

unsigned int bengine::Gamepad::GetIndex() const {
    return m_pImpl->GetIndex();
}
