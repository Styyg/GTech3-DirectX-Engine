#pragma once
#include "d3dUtil.h"
#include <array>
#include <vector>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	vector<Vertex> mVertices;
	vector<std::uint16_t> mIndices;
	ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
	unique_ptr<MeshGeometry> mMeshGeo = nullptr;

public:
	void CreateTriangle3D();
private:
	void CreateMesh();

};

