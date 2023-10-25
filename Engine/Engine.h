#pragma once

#include <windows.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <dxgi1_4.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class Engine
{
public:
    Engine(HINSTANCE hInstance);
    virtual ~Engine();

    int Run();
    void Update();

    bool InitMainWindow();
    void InitD3D();
    void SynchroProcess();
    void SetMSAA();
    void CreateCommandQueue();
    void CreateCommandList();
    void CreateCommandAllocator();
    void CreateRtvAndDsvDescriptorHeaps();


    // Utils
    void ThrowIfFailed(HRESULT hr);

private:
    HINSTANCE mhInst = nullptr;
    HWND mhWnd = nullptr;
    ComPtr<ID3D12Device> mD3DDevice;
    ComPtr<IDXGIFactory4> mDxgiFactory;
    ComPtr<IDXGISwapChain> mSwapChain;

    ComPtr<ID3D12Fence> mFence;
    UINT64 mFenceValue = 0;

    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12CommandQueue> mCommandQueue;

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

    ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    int mClientWidth = 800;
    int mClientHeight = 600;

    int mCurrentBackBuffer = 0;
};

