#pragma once
#include <memory>

namespace bengine {
    class Gamepad {
    public:
        enum class Button : unsigned int {
            DpadUp = 0x0001,
            DpadDown = 0x0002,
            DpadLeft = 0x0004,
            DpadRight = 0x0008,
            Start = 0x0010,
            Back = 0x0020,
            LeftThumb = 0x0040,
            RightThumb = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,
            A = 0x1000,
            B = 0x2000,
            X = 0x4000,
            Y = 0x8000
        };

        explicit Gamepad(unsigned int controllerIndex);
        ~Gamepad();
        Gamepad(Gamepad &&) noexcept;
        Gamepad &operator=(Gamepad &&) noexcept;
        Gamepad(const Gamepad &) = delete;
        Gamepad &operator=(const Gamepad &) = delete;

        void Update();
        [[nodiscard]] bool IsDown(Button button) const;
        [[nodiscard]] bool IsUp(Button button) const;
        [[nodiscard]] bool IsPressed(Button button) const;
        [[nodiscard]] unsigned int GetIndex() const;

    private:
        class GamepadImpl;
        std::unique_ptr<GamepadImpl> m_pImpl;
    };
}
