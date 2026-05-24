#pragma once

#include "InputHandler.h"
#include "Win32API.h"
#include <cstdint>
#include <functional>
#include <string_view>

struct WindowDesc
{
    std::wstring Title  = L"Untitled";
    uint32_t     Width  = 1280;
    uint32_t     Height = 720;
};

class Window final
{
  public:
    explicit Window(const WindowDesc& desc);
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = delete;
    Window& operator=(Window&&)      = delete;

    [[nodiscard]] bool ProcessMessages() noexcept;

    void SetResizeCallback(std::function<void(uint32_t, uint32_t)> cb) noexcept
    {
        m_ResizeCallback = std::move(cb);
    }

    [[nodiscard]] InputHandler& GetInputHandler() noexcept
    {
        return m_InputHandler;
    }

    [[nodiscard]] HWND GetNativeHandle() const noexcept
    {
        return m_WindowHandle;
    }

    [[nodiscard]] uint32_t GetWidth() const noexcept
    {
        return m_Width;
    }

    [[nodiscard]] uint32_t GetHeight() const noexcept
    {
        return m_Height;
    }

  private:
    static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  private:
    HWND         m_WindowHandle = nullptr;
    InputHandler m_InputHandler = {};
    uint32_t     m_Width        = 0;
    uint32_t     m_Height       = 0;

    std::function<void(uint32_t, uint32_t)> m_ResizeCallback;
};
