#include "DX11Presenter.h"
#include <cstring>
#include <d3d11.h>
#include <imgui_impl_dx11.h>
#include <stdexcept>

void DX11Presenter::Initialize(HWND nativeWindowHandle, uint32_t width, uint32_t height)
{
    if (!nativeWindowHandle)
        throw std::runtime_error("Invalid native window handle context provided.");

    m_Width  = width;
    m_Height = height;

    DXGI_SWAP_CHAIN_DESC scd               = {};
    scd.BufferDesc.Width                   = m_Width;
    scd.BufferDesc.Height                  = m_Height;
    scd.BufferDesc.RefreshRate.Numerator   = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.SampleDesc.Count                   = 1;
    scd.SampleDesc.Quality                 = 0;
    scd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount                        = 2;
    scd.OutputWindow                       = nativeWindowHandle;
    scd.Windowed                           = TRUE;
    scd.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               nullptr,
                                               0,
                                               featureLevels,
                                               1,
                                               D3D11_SDK_VERSION,
                                               &scd,
                                               m_SwapChain.GetAddressOf(),
                                               m_Device.GetAddressOf(),
                                               nullptr,
                                               m_DeviceContext.GetAddressOf());

    if (FAILED(hr))
        throw std::runtime_error("Failed to initialize core Direct3D device contexts.");

    Resize(m_Width, m_Height);
}

void DX11Presenter::Resize(uint32_t width, uint32_t height)
{
    if (!m_SwapChain)
        return;

    m_Width  = width;
    m_Height = height;

    m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_RenderTargetView.Reset();
    m_GPUTexture.Reset();

    HRESULT hr = m_SwapChain->ResizeBuffers(0, m_Width, m_Height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
        return;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

    hr = m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf());
    if (FAILED(hr))
        throw std::runtime_error("Failed to create render target view.");

    D3D11_TEXTURE2D_DESC td = {};
    td.Width                = m_Width;
    td.Height               = m_Height;
    td.MipLevels            = 1;
    td.ArraySize            = 1;
    td.Format               = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.SampleDesc.Count     = 1;
    td.SampleDesc.Quality   = 0;
    td.Usage                = D3D11_USAGE_DYNAMIC;
    td.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;

    hr = m_Device->CreateTexture2D(&td, nullptr, m_GPUTexture.GetAddressOf());
    if (FAILED(hr))
        throw std::runtime_error("Failed to create staging texture.");
}

void DX11Presenter::Present(std::span<const uint32_t> framebuffer, bool vsync)
{
    if (!m_SwapChain || !m_GPUTexture || framebuffer.empty())
        return;

    if (framebuffer.size() != static_cast<size_t>(m_Width) * m_Height)
        return;

    D3D11_MAPPED_SUBRESOURCE mapped = {};

    HRESULT hr = m_DeviceContext->Map(m_GPUTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (SUCCEEDED(hr))
    {
        uint8_t*       dst      = reinterpret_cast<uint8_t*>(mapped.pData);
        const uint8_t* src      = reinterpret_cast<const uint8_t*>(framebuffer.data());
        uint32_t       rowBytes = m_Width * sizeof(uint32_t);

        for (uint32_t y = 0; y < m_Height; ++y)
        {
            std::memcpy(dst + (y * mapped.RowPitch), src + (y * rowBytes), rowBytes);
        }
        m_DeviceContext->Unmap(m_GPUTexture.Get(), 0);
    }

    Microsoft::WRL::ComPtr<ID3D11Resource> srcResource;
    Microsoft::WRL::ComPtr<ID3D11Resource> dstResource;
    m_GPUTexture.As(&srcResource);
    m_RenderTargetView->GetResource(&dstResource);
    m_DeviceContext->CopyResource(dstResource.Get(), srcResource.Get());

    // ImGui stuff.
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    // End of ImGui Stuff

    m_SwapChain->Present(vsync ? 1 : 0, 0);
}
