#pragma once
#include <wrl.h>
#include "d3dx12.h"
#include "d3dUtil.h"
#include "GeometryGenerator.h"
#include "UploadBuffer.h"
#include "Transform.h"

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class Entity
{
public:
	Entity(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
		Microsoft::WRL::ComPtr<ID3D12Device> D3DDevice,
		Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO,
		Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CbvHeap);

	void BuildGeometry(); 
	void BuildConstantBuffers();
	void Update(XMMATRIX worldViewProj);
	void Render();
	void Translate(float x, float y, float z);

private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
	std::unique_ptr<MeshGeometry> mMeshGeo = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	Mesh mMesh;
	Transform mTransform;
};

