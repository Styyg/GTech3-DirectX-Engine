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
#include "Input.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "GeometryGenerator.h"
#include "PSOManager.h"
#include "GameTimer.h"

class GameObject;

struct ObjectConstants
{
    DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

struct PassConstants
{
    //DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
    //DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
    //DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
    //DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
    //DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
    //DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
    //float cbPerObjectPad1 = 0.0f;
    //DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
    //DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
    //float NearZ = 0.0f;
    //float FarZ = 0.0f;
    //float TotalTime = 0.0f;
    //float DeltaTime = 0.0f;
};

class Engine
{
public:
    Engine(HWND hWnd);
    virtual ~Engine();

    LONG GetClientWidth();
    LONG GetClientHeight();

    void Update(GameTimer gameTimer);
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
    void BuildRootSignature();
    void BuildTriangleGeometry();

    void CreateForm(GameObject* gameObject, int id, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    
    void FlushCommandQueue();

    ID3D12Resource* CurrentBackBuffer()const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

    void ResetCommandList();
    void CloseCommandeList();
    void ExecuteCommandList();
    void Flush();

    void BuildAllGameObjects();
    void DrawAllGameObjects();

    Camera* GetCam() { return &mCamera; }

private:
    ShaderManager shaderManager;
    
    HWND mHWnd = nullptr;

    bool      m4xMsaaState = false;
    UINT      m4xMsaaQuality = 0;

    UINT64 mFenceValue = 0;

    UINT mRtvDescriptorSize;
    UINT mDsvDescriptorSize;
    UINT mCbvSrvUavDescriptorSize;

    static const int mSwapChainBufferCount = 2;

    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
    Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<IDXGIFactory4> mDxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> mVsByteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> mPsByteCode = nullptr;

    UINT64 mCurrentFence = 0;

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;

    std::unique_ptr<MeshGeometry> mTriangleGeo = nullptr;

    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mWorldViewProj = MathHelper::Identity4x4(); // tranposed
    XMMATRIX mView;
    XMMATRIX mProj;

    std::vector<GameObject> mNewGameObbjects;
    // Generic PSO
    PSOManager mPsoManager;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC mBasePsoDesc = {};

    float mTheta = 1.5f * DirectX::XM_PI;
    float mPhi = DirectX::XM_PIDIV2;
    float mRadius = 0.0001f;

    Input input;
    Camera mCamera;

    int mClientWidth = 1280;
    int mClientHeight = 720;
    int mCurrentBackBuffer = 0;
};