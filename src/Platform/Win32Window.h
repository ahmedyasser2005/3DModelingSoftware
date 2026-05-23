#pragma once

#include "Input.h"
#include "Win32API.h"
#include <cstdint>
#include <functional>
#include <string_view>

struct WindowDesc
{
    std::wstring_view Title = L"No Window Title.";
    uint32_t Width = 1280;
    uint32_t Height = 720;
};

class Win32Window final
{
  public:
    explicit Win32Window(const WindowDesc& desc);
    ~Win32Window();

    Win32Window(const Win32Window&) = delete;
    Win32Window& operator=(const Win32Window&) = delete;
    Win32Window(Win32Window&&) = delete;
    Win32Window& operator=(Win32Window&&) = delete;

    [[nodiscard]] bool ProcessMessages() const noexcept;

    void SetResizeCallback(std::function<void(uint32_t, uint32_t)> cb) noexcept
    {
        m_ResizeCallback = std::move(cb);
    }

    [[nodiscard]] const Input& GetInput() const noexcept
    {
        return m_Input;
    }

    [[nodiscard]] HWND GetNativeHandle() const noexcept
    {
        return static_cast<HWND>(m_WindowHandle);
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
    Input m_Input;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;

    std::function<void(uint32_t, uint32_t)> m_ResizeCallback;

    void* m_WindowHandle = nullptr;
};
