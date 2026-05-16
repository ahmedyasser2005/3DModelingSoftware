#pragma once
#include <functional>
#include <memory>
#include <string_view>
#include <utility>

struct WindowImpl;

/**
 * @class Window
 * @brief Manages a native Win32 window instance using explicit RAII semantics.
 * * Handles the OS message pump, lifecycle management, and window resizing hooks.
 * This class cannot be copied or moved due to its strict underlying resource ownership.
 */
class [[nodiscard]] Window
{
  public:
    /**
     * @brief Spawns and registers a native Win32 window instance.
     * @param width The target width of the client workspace area in pixels.
     * @param height The target height of the client workspace area in pixels.
     * @param title A Unicode string view representing the window title.
     * @throws std::system_error If window class registration or instantiation fails.
     */
    Window(int width, int height, std::wstring_view title);

    /**
     * @brief Destroys the native HWND handle and unregisters the Win32 window class.
     */
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) noexcept = delete;
    Window &operator=(Window &&) noexcept = delete;

    /**
     * @brief Dispatches queued Win32 messages (Input, Resize, Close, etc.).
     * * Must be called non-blocking once at the beginning of each frame in the application loop.
     */
    void ProcessMessages() noexcept;

    /**
     * @brief Polls whether the OS or the user requested the application window to terminate.
     * @return True if the window is currently in the process of closing, false otherwise.
     */
    [[nodiscard]] bool ShouldClose() const noexcept;

    /**
     * @brief Exposes the raw abstract native HWND handle for hardware binding.
     * @return An abstract pointer targeting the native Win32 window workspace.
     */
    [[nodiscard]] void *GetNativeHandle() const noexcept;

    /**
     * @brief Retrieves the current client area dimensions of the window workspace.
     * @return A std::pair containing the width and height as uint32_t values.
     */
    [[nodiscard]] std::pair<uint32_t, uint32_t> GetDimensions() const noexcept;

    /**
     * @brief Queries and consumes the internal window mutation state flag.
     * @return True if a size mutation was processed since the last check, false otherwise.
     */
    [[nodiscard]] bool ConsumeResizeFlag() noexcept;

    /**
     * @brief Registers a thread callback to execute frame rendering during modal sizing loops.
     * * This prevents the pipeline from freezing completely when a user manually stalls
     * the message pump by holding or dragging the window borders.
     * * @param callback A callable destination signature matching void(uint32_t, uint32_t).
     */
    void SetResizeCallback(std::function<void(uint32_t, uint32_t)> callback) noexcept;

  private:
    std::unique_ptr<WindowImpl> m_impl;
};