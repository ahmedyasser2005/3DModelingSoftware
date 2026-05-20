#pragma once

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

class Input
{
  public:
    virtual ~Input() = default;

    virtual bool IsKeyPressed(KeyCode key) = 0;
    virtual bool IsKeyReleased(KeyCode key) = 0;

    virtual bool IsMouseButtonPressed(MouseButton button) = 0;
    virtual bool IsMouseButtonReleased(MouseButton button) = 0;
    virtual bool IsMouseMove(int32_t x, int32_t y) = 0;
    virtual bool IsMouseScroll(float delta) = 0;
};
