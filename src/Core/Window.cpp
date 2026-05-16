#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "Window.h"
#include <Windows.h>
#include <system_error>

struct WindowImpl
{
    HWND hwnd{nullptr};
    HINSTANCE hinstance{nullptr};
    std::wstring class_name;
    uint32_t width{0};
    uint32_t height{0};
    bool should_close{false};
    bool was_resized{false};
    std::function<void(uint32_t, uint32_t)> resize_callback{nullptr};
};

static LRESULT CALLBACK WindowProcedureSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW *const create_struct = reinterpret_cast<CREATESTRUCTW *>(lparam);
        WindowImpl *const impl = static_cast<WindowImpl *>(create_struct->lpCreateParams);

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(impl));
        impl->hwnd = hwnd;

        WNDPROC runtime_proc = [](HWND h, UINT m, WPARAM w, LPARAM l) noexcept -> LRESULT {
            WindowImpl *const state = reinterpret_cast<WindowImpl *>(GetWindowLongPtrW(h, GWLP_USERDATA));
            if (state) [[likely]]
            {
                switch (m)
                {
                case WM_CLOSE:
                    state->should_close = true;
                    return 0;

                case WM_SIZE:
                    if (w != SIZE_MINIMIZED)
                    {
                        state->width = LOWORD(l);
                        state->height = HIWORD(l);
                        state->was_resized = true;

                        if (state->resize_callback)
                        {
                            state->resize_callback(state->width, state->height);
                        }
                    }
                    return 0;

                case WM_SETCURSOR:
                    if (LOWORD(l) == HTCLIENT)
                    {
                        SetCursor(LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(static_cast<ULONG_PTR>(32512))));
                        return TRUE;
                    }
                    break;
                }
            }
            return DefWindowProcW(h, m, w, l);
        };
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(runtime_proc));
        return runtime_proc(hwnd, msg, wparam, lparam);
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

Window::Window(int width, int height, std::wstring_view title) : m_impl(std::make_unique<WindowImpl>())
{
    m_impl->hinstance = GetModuleHandleW(nullptr);
    m_impl->class_name = L"CustomRuntimeWindowClass";
    m_impl->width = static_cast<uint32_t>(width);
    m_impl->height = static_cast<uint32_t>(height);

    WNDCLASSEXW wc{.cbSize = sizeof(WNDCLASSEXW),
                   .style = CS_OWNDC,
                   .lpfnWndProc = WindowProcedureSetup,
                   .hInstance = m_impl->hinstance,
                   .lpszClassName = m_impl->class_name.c_str()};

    if (!RegisterClassExW(&wc))
    {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "Failed to register Win32 class");
    }

    RECT wr{0, 0, width, height};
    constexpr DWORD window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    AdjustWindowRectEx(&wr, window_style, FALSE, 0);

    HWND hwnd =
        CreateWindowExW(0, m_impl->class_name.c_str(), title.data(), window_style, CW_USEDEFAULT, CW_USEDEFAULT,
                        wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, m_impl->hinstance, m_impl.get());

    if (!hwnd)
    {
        UnregisterClassW(m_impl->class_name.c_str(), m_impl->hinstance);
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "Failed to instantiate Win32 handle");
    }
}

Window::~Window()
{
    if (m_impl && m_impl->hwnd)
    {
        DestroyWindow(m_impl->hwnd);
        UnregisterClassW(m_impl->class_name.c_str(), m_impl->hinstance);
    }
}

void Window::ProcessMessages() noexcept
{
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

bool Window::ShouldClose() const noexcept
{
    return m_impl->should_close;
}
void *Window::GetNativeHandle() const noexcept
{
    return m_impl->hwnd;
}
std::pair<uint32_t, uint32_t> Window::GetDimensions() const noexcept
{
    return {m_impl->width, m_impl->height};
}
bool Window::ConsumeResizeFlag() noexcept
{
    return std::exchange(m_impl->was_resized, false);
}

void Window::SetResizeCallback(std::function<void(uint32_t, uint32_t)> callback) noexcept
{
    m_impl->resize_callback = std::move(callback);
}