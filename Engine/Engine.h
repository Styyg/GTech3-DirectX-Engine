#pragma once

#include <windows.h>
#include <windef.h>
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
    Engine(HWND hWnd);
    virtual ~Engine();

    void Update();

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
    void BuildShadersAndInputLayout();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildTriangleGeometry();
    void BuildPSO();

    LONG GetClientWidth();
    LONG GetClientHeight();

    ID3D12Resource* CurrentBackBuffer()const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

private:
    ShaderManager shaderManager;
    
    //HINSTANCE mhInst = nullptr;
    HWND mHWnd = nullptr;

    // Set true to use 4X MSAA (�4.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

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
    ComPtr<ID3D12DescriptorHeap> mCbvHeap;
    ComPtr<ID3D12PipelineState> mPSO = nullptr;
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3DBlob> mVsByteCode = nullptr;
    ComPtr<ID3DBlob> mPsByteCode = nullptr;

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

    vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;

    std::unique_ptr<MeshGeometry> mTriangleGeo = nullptr;


    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    //int mClientWidth = 800;
    //int mClientHeight = 600;
    int mCurrentBackBuffer = 0;
};