#pragma once
#include <array>
#include <cstdint>
#include <memory>

struct RendererImpl;

/**
 * @class Renderer
 * @brief Handles the low-level Direct3D 11 hardware execution pipeline.
 * * Manages the swapchain layouts, dynamic CPU-to-GPU texture uploads, and surface target views.
 */
class [[nodiscard]] Renderer
{
  public:
    /**
     * @brief Initializes the hardware graphics device context bound to a native handle.
     * @param native_window_handle Opaque abstract pointer to the underlying HWND.
     * @throws std::runtime_error If a valid swapchain layer or hardware device layer fails to initialize.
     */
    explicit Renderer(void *native_window_handle);

    /**
     * @brief Releases all Direct3D 11 resource views and runtime interfaces securely.
     */
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) noexcept = delete;
    Renderer &operator=(Renderer &&) noexcept = delete;

    /**
     * @brief Mutates the backbuffer swapchain allocation topology to match updated client boundaries.
     * @param width New swapchain backbuffer target width in pixels.
     * @param height New swapchain backbuffer target height in pixels.
     */
    void Resize(uint32_t width, uint32_t height);

    /**
     * @brief Clears the active swapchain render target view to a uniform color structure.
     * @param color An RGBA color float layout mapping array normalized from 0.0f to 1.0f.
     */
    void Clear(const std::array<float, 4> &color) noexcept;

    /**
     * @brief Submits linear software buffer contents directly to a dynamic GPU texture view.
     * @param pixelData Pointer to contiguous raw linear uint32_t buffer elements (Format: BGRA).
     * @param width Source canvas pitch scale configuration width component.
     * @param height Source canvas pitch scale configuration height component.
     */
    void CopyFramebufferToScreen(const uint32_t *pixelData, uint32_t width, uint32_t height) noexcept;

    /**
     * @brief Commits the backbuffer sequence to the presentation display surface layer.
     */
    void Present() noexcept;

    /**
     * @brief Exposes the underlying primary Direct3D hardware device handle.
     * @return Opaque pointer referencing the active ID3D11Device.
     */
    [[nodiscard]] struct ID3D11Device *GetDevice() const noexcept;

    /**
     * @brief Exposes the underlying device direct rendering sequence context handle.
     * @return Opaque pointer referencing the active ID3D11DeviceContext.
     */
    [[nodiscard]] struct ID3D11DeviceContext *GetContext() const noexcept;

  private:
    std::unique_ptr<RendererImpl> m_impl;
};