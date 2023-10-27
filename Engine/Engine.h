#pragma once

#include <windows.h>
#include "d3dx12.h"
#include <D3Dcompiler.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>
#include "d3dUtil.h"

#include "ShaderManager.h"

using namespace Microsoft::WRL;
using namespace std;

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
    void SwapChain();
    void CreateRtvAndDsvDescriptorHeaps();
    void RenderTargetView();
    void DescribeDepthStencilBuffer();
    void SetupGraphicsPipeline();
    void BuildInputLayout();
    void RootSignature();

    ID3D12Resource* CurrentBackBuffer()const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

private:
    ShaderManager shaderManager;
    
    HINSTANCE mhInst = nullptr;
    HWND mhWnd = nullptr;

    UINT64 mFenceValue = 0;

    UINT mRtvDescriptorSize;
    UINT mDsvDescriptorSize;
    UINT mCbvSrvUavDescriptorSize;

    static const int mSwapChainBufferCount = 2;

    ComPtr<ID3D12Fence> mFence;
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12CommandQueue> mCommandQueue;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    ComPtr<ID3D12Device> mD3DDevice;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<IDXGIFactory4> mDxgiFactory;
    ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    ComPtr<ID3D12DescriptorHeap> mDsvHeap;	
    ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
    ComPtr<ID3D12Resource> mDepthStencilBuffer;
    ComPtr<ID3D12RootSignature> mRootSignature;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap;

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

    vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutDesc;

    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;

    int mClientWidth = 800;
    int mClientHeight = 600;
    int mCurrentBackBuffer = 0;
};