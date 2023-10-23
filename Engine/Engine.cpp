#include "Engine.h"

Engine::Engine()
{
    if (!InitD3D())
    {
        MessageBox(nullptr, L"Failed to initialize Direct3D", L"Error", MB_OK);
    }

    SynchroProcess();
}

Engine::~Engine()
{
    
}

bool Engine::InitD3D() {
    HRESULT hr = D3D12CreateDevice(
        nullptr,                // pAdapter
        D3D_FEATURE_LEVEL_11_0, // Minimum feature level this app can support
        IID_PPV_ARGS(&mD3DDevice)
    );

    return SUCCEEDED(hr);
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