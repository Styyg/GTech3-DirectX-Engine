#pragma once

#include <stdexcept>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class Engine
{
public:
	Engine();
	~Engine();

	bool InitD3D();
	void SynchroProcess();

	void Update();

	// Utils
	void ThrowIfFailed(HRESULT hr);

private:
	Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

	UINT64 mFenceValue = 0;
};