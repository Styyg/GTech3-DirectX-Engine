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
#include "UploadBuffer.h"
#include "ShaderManager.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class Engine
{
public:
    Engine(HWND hWnd);
    virtual ~Engine();

    void Update();
    void Draw();
    void OnResize();

    void InitD3D();
    void SynchroProcess();
    void SetMSAA();
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateDescriptorHeaps();

    void RenderTargetView();
    void DescribeDepthStencilBuffer();
    void BuildShadersAndInputLayout();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildTriangleGeometry();
    void BuildPSO(); 
    
    void FlushCommandQueue();

    LONG GetClientWidth();
    LONG GetClientHeight();

    ID3D12Resource* CurrentBackBuffer()const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

private:
    ShaderManager shaderManager;
    
    HWND mHWnd = nullptr;

    // Set true to use 4X MSAA (�4.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

    UINT64 mFenceValue = 0;

    UINT mRtvDescriptorSize;
    UINT mDsvDescriptorSize;
    UINT mCbvSrvUavDescriptorSize;

    static const int mSwapChainBufferCount = 2;

    unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

    ComPtr<ID3D12Fence> mFence;
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12CommandQueue> mCommandQueue;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    ComPtr<ID3D12Device> mD3DDevice;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<IDXGIFactory4> mDxgiFactory;
    ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    ComPtr<ID3D12DescriptorHeap> mDsvHeap;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap;
    ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
    ComPtr<ID3D12Resource> mDepthStencilBuffer;
    ComPtr<ID3D12PipelineState> mPSO = nullptr;
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3DBlob> mVsByteCode = nullptr;
    ComPtr<ID3DBlob> mPsByteCode = nullptr;

    UINT64 mCurrentFence = 0;

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

    vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;

    unique_ptr<MeshGeometry> mTriangleGeo = nullptr;

    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    int mClientWidth = 800;
    int mClientHeight = 600;
    int mCurrentBackBuffer = 0;
};