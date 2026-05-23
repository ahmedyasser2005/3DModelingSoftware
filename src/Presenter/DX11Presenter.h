#pragma once

#include "Platform/Win32API.h"
#include <cstdint>
#include <d3d11.h>
#include <span>
#include <vector>
#include <wrl/client.h>

class DX11Presenter final
{
  public:
    DX11Presenter() = default;
    ~DX11Presenter() = default;
    DX11Presenter(const DX11Presenter&) = delete;
    DX11Presenter& operator=(const DX11Presenter&) = delete;
    DX11Presenter(DX11Presenter&&) = delete;
    DX11Presenter& operator=(DX11Presenter&&) = delete;

    void Initialize(HWND nativeWindowHandle, uint32_t width, uint32_t height);
    void Resize(uint32_t width, uint32_t height);

    void Present(std::span<const uint32_t> framebuffer, bool vsync);

  private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_GPUTexture;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};
