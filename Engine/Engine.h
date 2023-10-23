#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class Engine
{
public:
	Engine();
	~Engine();

	bool InitializeD3D();

	void Update();

private:
	Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
};