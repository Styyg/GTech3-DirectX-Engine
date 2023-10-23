#include "Engine.h"

Engine::Engine()
{
    if (!InitializeD3D())
    {
        MessageBox(nullptr, L"Failed to initialize Direct3D", L"Error", MB_OK);
    }
}

Engine::~Engine()
{
    
}

bool Engine::InitializeD3D() {
    HRESULT hr = D3D12CreateDevice(
        nullptr,                // pAdapter
        D3D_FEATURE_LEVEL_11_0, // Minimum feature level this app can support
        IID_PPV_ARGS(&mD3DDevice)
    );

    return SUCCEEDED(hr);
}

void Engine::Update()
{

}