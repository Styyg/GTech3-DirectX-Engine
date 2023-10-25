#pragma once

#include <stdexcept>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class Engine
{
public:
	Engine();
	~Engine();

	void InitD3D();
	void SynchroProcess();
	void SetMSAA();
	void CreateCommandQueue();
	void CreateCommandList();
	void CreateCommandAllocator();

	void Update();

	// Utils
	void ThrowIfFailed(HRESULT hr);

private:
	ComPtr<ID3D12Device> mD3DDevice;
	ComPtr<ID3D12Fence> mFence;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	UINT64 mFenceValue = 0;

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
};