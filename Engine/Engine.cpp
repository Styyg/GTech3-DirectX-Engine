#include "Engine.h"

Engine::Engine()
{
    InitD3D();
    SynchroProcess();
    SetMSAA();
    CreateCommandAllocator();
    CreateCommandList();
    CreateCommandQueue();
}

Engine::~Engine()
{

}

void Engine::InitD3D() {
    HRESULT hr = D3D12CreateDevice(
        nullptr,                // pAdapter
        D3D_FEATURE_LEVEL_11_0, // Minimum feature level this app can support
        IID_PPV_ARGS(&mD3DDevice)
    );

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"Failed to initialize Direct3D", L"Error", MB_OK);
    }
}

void Engine::SynchroProcess()
{
    HRESULT hr = mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
    if (FAILED(hr))
    {
        ThrowIfFailed(hr);
    }

    UINT mRtvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    UINT mDsvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    UINT mCbvSrvUavDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Engine::SetMSAA()
{
    msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;

    ThrowIfFailed(mD3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,&msQualityLevels,sizeof(msQualityLevels)));

    if (msQualityLevels.NumQualityLevels == 0) {
        MessageBox(0, L"4x MSAA n'est pas supporté sur votre appareil", L"Erreur", MB_OK);

        // ex : 1x MSAA ou pas de MSAA
    }
}

void Engine::CreateCommandAllocator()
{
    HRESULT hr = mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
    if (FAILED(hr))
    {
        ThrowIfFailed(hr);
    }
}

void Engine::CreateCommandList()
{
    HRESULT hr = mD3DDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mCommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&mCommandList)
    );
    if (FAILED(hr))
    {
        ThrowIfFailed(hr);
    }
}

void Engine::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    HRESULT hr = mD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
    if (FAILED(hr))
    {
        ThrowIfFailed(hr);
    }
}

void Engine::Update()
{

}

void Engine::ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::runtime_error("A Direct3D operation failed with code: " + std::to_string(hr));
    }
}