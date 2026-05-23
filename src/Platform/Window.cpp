#include "Window.h"
#include <stdexcept>

static constexpr UINT_PTR RESIZE_TIMER_ID = 1;

static KeyCode MapVK(WPARAM vk) noexcept
{
    if (vk >= 'A' && vk <= 'Z')
        return static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::A) + (vk - 'A'));
    if (vk >= '0' && vk <= '9')
        return static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::D0) + (vk - '0'));

    switch (vk)
    {
    case VK_ESCAPE:
        return KeyCode::Escape;
    case VK_SPACE:
        return KeyCode::Space;
    case VK_RETURN:
        return KeyCode::Enter;
    case VK_BACK:
        return KeyCode::Backspace;
    case VK_TAB:
        return KeyCode::Tab;
    case VK_SHIFT:
        return KeyCode::Shift;
    case VK_CONTROL:
        return KeyCode::Control;
    case VK_MENU:
        return KeyCode::Alt;
    default:
        return KeyCode::Unknown;
    }
}

LRESULT CALLBACK Window::WindowProcThunk(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        pWindow->m_WindowHandle = hwnd;
    }

    if (pWindow)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        // Start of Timer method of "Rendering While Resizing"
        /* The way I handled "rendering while resizing the window"
         * is about making a timer message that is sent every 10ms
         * so every 10 ms the window gets redrawn (while resizing).
         */
        case WM_ENTERSIZEMOVE:
        {
            SetTimer(hwnd, RESIZE_TIMER_ID, 10, nullptr); // ~10 ms tick
            return 0;
        }
        case WM_EXITSIZEMOVE:
        {
            KillTimer(hwnd, RESIZE_TIMER_ID);
            return 0;
        }
        case WM_TIMER:
        {
            if (wParam != RESIZE_TIMER_ID)
                break;

            RECT rc = {};
            GetClientRect(hwnd, &rc);
            uint32_t newW = static_cast<uint32_t>(rc.right - rc.left);
            uint32_t newH = static_cast<uint32_t>(rc.bottom - rc.top);

            if (newW > 0 && newH > 0 && (newW != pWindow->m_Width || newH != pWindow->m_Height))
            {
                pWindow->m_Width = newW;
                pWindow->m_Height = newH;
                if (pWindow->m_ResizeCallback)
                    pWindow->m_ResizeCallback(newW, newH);
            }
            return 0;
        }
        case WM_SIZE:
        {
            uint32_t newW = static_cast<uint32_t>(LOWORD(lParam));
            uint32_t newH = static_cast<uint32_t>(HIWORD(lParam));
            if (newW > 0 && newH > 0 && (newW != pWindow->m_Width || newH != pWindow->m_Height))
            {
                pWindow->m_Width = newW;
                pWindow->m_Height = newH;
                if (pWindow->m_ResizeCallback)
                    pWindow->m_ResizeCallback(newW, newH);
            }
            return 0;
        } // End of Timer method of "Rendering While Resizing"

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            pWindow->m_Input.UpdateKeyState(MapVK(wParam), true);
            return 0;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            pWindow->m_Input.UpdateKeyState(MapVK(wParam), false);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            int32_t x = static_cast<int32_t>(LOWORD(lParam));
            int32_t y = static_cast<int32_t>(HIWORD(lParam));
            pWindow->m_Input.UpdateMousePosition(x, y);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Left, true);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Left, false);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Right, true);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Right, false);
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Middle, true);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            pWindow->m_Input.UpdateMouseState(MouseButton::Middle, false);
            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

Window::Window(const WindowDesc& desc) : m_Width(desc.Width), m_Height(desc.Height)
{
    HINSTANCE hInstance = GetModuleHandleW(nullptr);
    const wchar_t* className = L"3DModelingSoftwareWindowClass";

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcThunk;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = className;

    RegisterClassExW(&wc);

    RECT wr = { 0, 0, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowExW(0,
                                className,
                                desc.Title.data(),
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                wr.right - wr.left,
                                wr.bottom - wr.top,
                                nullptr,
                                nullptr,
                                hInstance,
                                this);

    if (!hWnd)
        throw std::runtime_error("Failed to construct system window instance handle.");

    m_WindowHandle = hWnd;
    ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
    if (m_WindowHandle)
        DestroyWindow(reinterpret_cast<HWND>(m_WindowHandle));
}

bool Window::ProcessMessages() noexcept
{
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}
