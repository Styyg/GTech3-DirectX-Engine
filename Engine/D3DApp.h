#pragma once

#include <windows.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <dxgi1_4.h>
#include <wrl.h>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance);
    virtual ~D3DApp();

    virtual bool Initialize();
    int Run();

private:
    bool InitMainWindow();
    bool InitDirect3D();

    HINSTANCE mhInst = nullptr;
    HWND mhWnd = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

    int mClientWidth = 800;
    int mClientHeight = 600;
};

