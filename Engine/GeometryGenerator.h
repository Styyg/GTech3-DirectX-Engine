#pragma once
#include "d3dUtil.h"
#include <array>
#include <vector>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;


class GeometryGenerator
{
public:
	GeometryGenerator();
	~GeometryGenerator();

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct Mesh
	{
		vector<Vertex> vertices;
		vector<uint16_t> indices;
	};

	Mesh CreateTriangle3D(float width, float height, float depth);

private:
	//ComPtr<ID3D12Device>* mDevicePtr;
	//ComPtr<ID3D12GraphicsCommandList>* mCommandListPtr;

	ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
	unique_ptr<MeshGeometry> mMeshGeo = nullptr;

//private:
	//void CreateMesh();

};

