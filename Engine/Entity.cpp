#include "Entity.h"

Entity::Entity(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
			Microsoft::WRL::ComPtr<ID3D12Device> D3DDevice,
			Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO,
			Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature,
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CbvHeap)
{
	mCommandList = commandList;
	mD3DDevice = D3DDevice;
	mPSO = PSO;
	mRootSignature = RootSignature;
	mCbvHeap = CbvHeap;

	BuildGeometry();
	BuildConstantBuffers();
}

void Entity::Update(XMMATRIX worldViewProj)
{
	ObjectConstants objConstants;
	//XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj * XMMatrixTranslation(0.5, 0, 0)));
	//mObjectCB->CopyData(0, objConstants);

	XMFLOAT4X4 world = mTransform.mWorldMatrix;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj * XMLoadFloat4x4(&world)));
	mObjectCB->CopyData(0, objConstants);
}

void Entity::Render()
{
	// shader
	mCommandList->SetPipelineState(mPSO.Get());
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
	mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	// geo
	mCommandList->IASetVertexBuffers(0, 1, &mMeshGeo->VertexBufferView());
	mCommandList->IASetIndexBuffer(&mMeshGeo->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw
	mCommandList->DrawIndexedInstanced(mMeshGeo->DrawArgs["cube"].IndexCount, 1, 0, 0, 0);
}

void Entity::Translate(float x, float y, float z)
{
	mTransform.TranslateInWorld(x, y, z);
}

void Entity::BuildGeometry()
{
	GeometryGenerator geoGen;
	Mesh mMesh = geoGen.CreateCube(1.0f, 1.0f, 1.0f);

	const UINT vbByteSize = (UINT)mMesh.vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)mMesh.indices.size() * sizeof(std::uint16_t);

	mMeshGeo = std::make_unique<MeshGeometry>();
	mMeshGeo->Name = "cubeGeo";

	mMeshGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), mMesh.vertices.data(), vbByteSize, mMeshGeo->VertexBufferUploader);

	mMeshGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), mMesh.indices.data(), ibByteSize, mMeshGeo->IndexBufferUploader);

	mMeshGeo->VertexByteStride = sizeof(Vertex);
	mMeshGeo->VertexBufferByteSize = vbByteSize;
	mMeshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mMeshGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)mMesh.indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mMeshGeo->DrawArgs["cube"] = submesh;
}

void Entity::BuildConstantBuffers()
{
	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(mD3DDevice.Get(), 1, true);

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * objCBByteSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	mD3DDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}
