#pragma once
#include "d3dUtil.h"
#include <array>
#include <vector>
#include "Mesh.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;


class GeometryGenerator
{
public:
	GeometryGenerator();
	~GeometryGenerator();

	Mesh CreateTriangle3D(float width, float height, float depth);

private:
	ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
	unique_ptr<MeshGeometry> mMeshGeo = nullptr;
};

