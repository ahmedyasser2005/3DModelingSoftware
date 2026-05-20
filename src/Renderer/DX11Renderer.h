#pragma once

#include "Platform/Win32API.h"
#include <cstdint>
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

class DX11Renderer final
{
  public:
    DX11Renderer() = default;
    ~DX11Renderer() = default;
    DX11Renderer(const DX11Renderer&) = delete;
    DX11Renderer& operator=(const DX11Renderer&) = delete;
    DX11Renderer(DX11Renderer&&) = delete;
    DX11Renderer& operator=(DX11Renderer&&) = delete;

    void Initialize(HWND nativeWindowHandle, uint32_t width, uint32_t height);
    void Resize(uint32_t width, uint32_t height);

    void Present(const std::vector<uint32_t>& framebuffer, bool vsync);

  private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_GPUTexture;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};