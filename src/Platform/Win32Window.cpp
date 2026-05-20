#include "Win32Window.h"
#include <stdexcept>

static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window* pWindow = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pWindow = reinterpret_cast<Win32Window*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        pWindow->m_WindowHandle = hwnd;
    }

    if (pWindow && pWindow->m_Input)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
            pWindow->m_Width = static_cast<uint32_t>(LOWORD(lParam));
            pWindow->m_Height = static_cast<uint32_t>(HIWORD(lParam));
            return 0;
        }

        // Keyboard Input Processing
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            // Translate basic alphanumeric keys 'A' through 'Z'
            if (wParam >= 'A' && wParam <= 'Z')
            {
                KeyCode key = static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::A) + (wParam - 'A'));
                pWindow->m_Input->IsKeyPressed(key);
            }
            // Translate basic digit keys '0' through '9'
            else if (wParam >= '0' && wParam <= '9')
            {
                KeyCode key = static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::D0) + (wParam - '0'));
                pWindow->m_Input->IsKeyPressed(key);
            }
            else
            {
                switch (wParam)
                {
                case VK_ESCAPE:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Escape);
                    break;
                case VK_SPACE:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Space);
                    break;
                case VK_RETURN:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Enter);
                    break;
                case VK_BACK:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Backspace);
                    break;
                case VK_TAB:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Tab);
                    break;
                case VK_SHIFT:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Shift);
                    break;
                case VK_CONTROL:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Control);
                    break;
                case VK_MENU:
                    pWindow->m_Input->IsKeyPressed(KeyCode::Alt);
                    break;
                }
            }
            return 0;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            if (wParam >= 'A' && wParam <= 'Z')
            {
                KeyCode key = static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::A) + (wParam - 'A'));
                pWindow->m_Input->IsKeyReleased(key);
            }
            else if (wParam >= '0' && wParam <= '9')
            {
                KeyCode key = static_cast<KeyCode>(static_cast<uint8_t>(KeyCode::D0) + (wParam - '0'));
                pWindow->m_Input->IsKeyReleased(key);
            }
            else
            {
                switch (wParam)
                {
                case VK_ESCAPE:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Escape);
                    break;
                case VK_SPACE:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Space);
                    break;
                case VK_RETURN:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Enter);
                    break;
                case VK_BACK:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Backspace);
                    break;
                case VK_TAB:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Tab);
                    break;
                case VK_SHIFT:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Shift);
                    break;
                case VK_CONTROL:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Control);
                    break;
                case VK_MENU:
                    pWindow->m_Input->IsKeyReleased(KeyCode::Alt);
                    break;
                }
            }
            return 0;
        }

        // Mouse Input Processing
        case WM_MOUSEMOVE:
        {
            int32_t x = static_cast<int32_t>(LOWORD(lParam));
            int32_t y = static_cast<int32_t>(HIWORD(lParam));
            pWindow->m_Input->IsMouseMove(x, y);
            return 0;
        }
        case WM_LBUTTONDOWN:
            pWindow->m_Input->IsMouseButtonPressed(MouseButton::Left);
            return 0;
        case WM_LBUTTONUP:
            pWindow->m_Input->IsMouseButtonReleased(MouseButton::Left);
            return 0;
        case WM_RBUTTONDOWN:
            pWindow->m_Input->IsMouseButtonPressed(MouseButton::Right);
            return 0;
        case WM_RBUTTONUP:
            pWindow->m_Input->IsMouseButtonReleased(MouseButton::Right);
            return 0;
        case WM_MBUTTONDOWN:
            pWindow->m_Input->IsMouseButtonPressed(MouseButton::Middle);
            return 0;
        case WM_MBUTTONUP:
            pWindow->m_Input->IsMouseButtonReleased(MouseButton::Middle);
            return 0;

        case WM_MOUSEWHEEL:
        {
            float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
            pWindow->m_Input->IsMouseScroll(delta);
            return 0;
        }
        }
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

Win32Window::Win32Window(const WindowDesc& desc, Input& inputMiddleman)
    : m_Width(desc.Width), m_Height(desc.Height), m_Input(&inputMiddleman)
{
    HINSTANCE hInstance = GetModuleHandleW(nullptr);
    m_InstanceHandle = reinterpret_cast<void*>(hInstance);

    const wchar_t* className = L"EngineWindowClass";

    WNDCLASSEXW wc = { .cbSize = sizeof(WNDCLASSEXW) };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
    {
        throw std::runtime_error("Failed to initialize standard Windows frame.");
    }

    m_WindowHandle = reinterpret_cast<void*>(hWnd);
    ShowWindow(hWnd, SW_SHOW);
}

Win32Window::~Win32Window()
{
    if (m_WindowHandle)
    {
        DestroyWindow(reinterpret_cast<HWND>(m_WindowHandle));
    }
}

bool Win32Window::ProcessMessages() const noexcept
{
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}
