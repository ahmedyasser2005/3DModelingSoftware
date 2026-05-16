#include "Renderer.h"
#include <cstring>
#include <d3d11.h>
#include <stdexcept>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct RendererImpl
{
    ComPtr<ID3D11Device> device{nullptr};
    ComPtr<ID3D11DeviceContext> context{nullptr};
    ComPtr<IDXGISwapChain> swap_chain{nullptr};
    ComPtr<ID3D11RenderTargetView> render_target_view{nullptr};
    ComPtr<ID3D11Texture2D> dynamic_texture{nullptr};
};

Renderer::Renderer(void *native_window_handle) : m_impl(std::make_unique<RendererImpl>())
{
    if (!native_window_handle)
    {
        throw std::runtime_error("Renderer initialization failed: Native window handle is null.");
    }

    HWND hwnd = static_cast<HWND>(native_window_handle);

    RECT rc;
    GetClientRect(hwnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC scd{.BufferDesc{.Width = width,
                                         .Height = height,
                                         .RefreshRate{.Numerator = 0, .Denominator = 1},
                                         .Format = DXGI_FORMAT_B8G8R8A8_UNORM},
                             .SampleDesc{.Count = 1, .Quality = 0},
                             .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
                             .BufferCount = 2,
                             .OutputWindow = hwnd,
                             .Windowed = TRUE,
                             .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
                             .Flags = 0};

    D3D_FEATURE_LEVEL feature_levels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
    D3D_FEATURE_LEVEL actual_feature_level;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creation_flags, feature_levels, ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION, &scd, &m_impl->swap_chain, &m_impl->device, &actual_feature_level, &m_impl->context);

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create Direct3D 11 device and swap chain.");
    }

    ComPtr<ID3D11Texture2D> back_buffer;
    m_impl->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(back_buffer.GetAddressOf()));

    hr = m_impl->device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_impl->render_target_view);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create Render Target View.");
    }
}

Renderer::~Renderer() = default;

void Renderer::Resize(uint32_t width, uint32_t height)
{
    if (!m_impl->device || !m_impl->swap_chain)
        return;

    m_impl->render_target_view.Reset();
    m_impl->context->OMSetRenderTargets(0, nullptr, nullptr);

    HRESULT hr = m_impl->swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    if (FAILED(hr))
    {
        throw std::runtime_error("Direct3D: Failed to resize swap chain buffers.");
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
    hr = m_impl->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                       reinterpret_cast<void **>(back_buffer.GetAddressOf()));
    if (SUCCEEDED(hr))
    {
        m_impl->device->CreateRenderTargetView(back_buffer.Get(), nullptr, &m_impl->render_target_view);
    }

    m_impl->dynamic_texture.Reset();
}

void Renderer::Clear(const std::array<float, 4> &color) noexcept
{
    if (m_impl->render_target_view)
    {
        m_impl->context->ClearRenderTargetView(m_impl->render_target_view.Get(), color.data());
    }
}

void Renderer::CopyFramebufferToScreen(const uint32_t *pixelData, uint32_t width, uint32_t height) noexcept
{
    if (!pixelData || width == 0 || height == 0) [[unlikely]]
        return;

    if (!m_impl->dynamic_texture)
    {
        D3D11_TEXTURE2D_DESC td{.Width = width,
                                .Height = height,
                                .MipLevels = 1,
                                .ArraySize = 1,
                                .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
                                .SampleDesc{.Count = 1, .Quality = 0},
                                .Usage = D3D11_USAGE_DYNAMIC,
                                .BindFlags = D3D11_BIND_SHADER_RESOURCE,
                                .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE};
        m_impl->device->CreateTexture2D(&td, nullptr, &m_impl->dynamic_texture);
    }

    D3D11_MAPPED_SUBRESOURCE mapped_res;
    HRESULT hr = m_impl->context->Map(m_impl->dynamic_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
    if (SUCCEEDED(hr))
    {
        uint8_t *dest = static_cast<uint8_t *>(mapped_res.pData);
        const uint8_t *src = reinterpret_cast<const uint8_t *>(pixelData);

        for (uint32_t y = 0; y < height; ++y)
        {
            std::memcpy(dest + (y * mapped_res.RowPitch), src + (y * width * sizeof(uint32_t)),
                        width * sizeof(uint32_t));
        }
        m_impl->context->Unmap(m_impl->dynamic_texture.Get(), 0);

        ComPtr<ID3D11Texture2D> back_buffer;
        hr = m_impl->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                           reinterpret_cast<void **>(back_buffer.GetAddressOf()));
        if (SUCCEEDED(hr))
        {
            m_impl->context->CopyResource(back_buffer.Get(), m_impl->dynamic_texture.Get());
        }
    }
}

void Renderer::Present() noexcept
{
    m_impl->swap_chain->Present(1, 0);
}

ID3D11Device *Renderer::GetDevice() const noexcept
{
    return m_impl->device.Get();
}
ID3D11DeviceContext *Renderer::GetContext() const noexcept
{
    return m_impl->context.Get();
}