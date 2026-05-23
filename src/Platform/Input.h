#pragma once

#include <array>
#include <bitset>
#include <cstdint>

enum class KeyCode : uint8_t
{
    Unknown = 0,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,
    Escape,
    Space,
    Enter,
    Backspace,
    Tab,
    Shift,
    Control,
    Alt
};

enum class MouseButton : uint8_t
{
    Left = 0,
    Right,
    Middle
};

class Input final
{
  public:
    Input() = default;
    ~Input() = default;

    // IsKeyDown(KeyCode::Unknown) is still callable
    // and returns garbage state. Same for others as well.

    [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept
    {
        return m_KeyStates[static_cast<size_t>(key)];
    }
    [[nodiscard]] bool IsKeyUp(KeyCode key) const noexcept
    {
        return !m_KeyStates[static_cast<size_t>(key)];
    }

    [[nodiscard]] bool IsMouseButtonDown(MouseButton button) const noexcept
    {
        return m_MouseStates[static_cast<size_t>(button)];
    }
    [[nodiscard]] bool IsMouseButtonUp(MouseButton button) const noexcept
    {
        return !m_MouseStates[static_cast<size_t>(button)];
    }

    // TODO: ...
    // [[nodiscard]] bool IsKeyJustPressed(KeyCode key) const noexcept;
    // [[nodiscard]] bool IsKeyJustReleased(KeyCode key) const noexcept;
    // [[nodiscard]] bool IsMouseButtonJustPressed(KeyCode key) const noexcept;
    // [[nodiscard]] bool IsMouseButtonJustReleased(KeyCode key) const noexcept;

    [[nodiscard]] int32_t GetMouseX() const noexcept
    {
        return m_MouseX;
    }
    [[nodiscard]] int32_t GetMouseY() const noexcept
    {
        return m_MouseY;
    }

    void UpdateKeyState(KeyCode key, bool isDown) noexcept
    {
        if (key != KeyCode::Unknown)
        {
            m_KeyStates[static_cast<size_t>(key)] = isDown;
        }
    }
    void UpdateMouseState(MouseButton button, bool isDown) noexcept
    {
        m_MouseStates[static_cast<size_t>(button)] = isDown;
    }
    void UpdateMousePosition(int32_t x, int32_t y) noexcept
    {
        m_MouseX = x;
        m_MouseY = y;
    }

  private:
    std::bitset<256> m_KeyStates = {};
    std::bitset<3> m_MouseStates = {};
    int32_t m_MouseX = 0;
    int32_t m_MouseY = 0;
};
