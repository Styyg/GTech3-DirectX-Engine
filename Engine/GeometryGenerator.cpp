#include "GeometryGenerator.h"

GeometryGenerator::GeometryGenerator()
{

}

GeometryGenerator::~GeometryGenerator()
{

}

GeometryGenerator::Mesh GeometryGenerator::CreateTriangle3D(float width, float height, float depth)
{
	Mesh mesh;

	float w = 0.5f * width;
	float h = 0.5f * height;
	float d = 0.5f * depth;

	mesh.vertices =
	{
		// front face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Red) }), // up 
		Vertex({ XMFLOAT3(+w, -h, 0.0f), XMFLOAT4(Colors::Red) }), // front right
		Vertex({ XMFLOAT3(-w, -h, 0.0f), XMFLOAT4(Colors::Red) }), // front left
		// left back face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Blue) }), // up 
		Vertex({ XMFLOAT3(-w, -h, 0.0f), XMFLOAT4(Colors::Blue) }), // front left
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Blue) }), // back middle
		// right back face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Green) }), // up 
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Green) }), // back middle
		Vertex({ XMFLOAT3(+w, -h, 0.0f), XMFLOAT4(Colors::Green) }), // front right
		// bottom face
		Vertex({ XMFLOAT3(+w, -h, 0.0f), XMFLOAT4(Colors::Green) }), // front right
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Blue) }), // back middle
		Vertex({ XMFLOAT3(-w, -h, 0.0f), XMFLOAT4(Colors::Red) }), // front left
	};

	mesh.indices =
	{
		// front face
		0, 1, 2,
		// back left face 
		3, 4, 5,
		// back right face 
		6, 7, 8,
		// bottom right face 
		9, 10, 11,
	};

	return mesh;
}

/*void GeometryGenerator::CreateMesh()
{
	const UINT vbByteSize = (UINT)mVertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)mIndices.size() * sizeof(uint16_t);

	mMeshGeo = std::make_unique<MeshGeometry>();
	mMeshGeo->Name = "triGeo";

	//mMeshGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevicePtr->Get(),
	//	mCommandListPtr->Get(), mVertices.data(), vbByteSize, mMeshGeo->VertexBufferUploader);

	//mMeshGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mDevicePtr->Get(),
	//	mCommandListPtr->Get(), mIndices.data(), ibByteSize, mMeshGeo->IndexBufferUploader);

	mMeshGeo->VertexByteStride = sizeof(Vertex);
	mMeshGeo->VertexBufferByteSize = vbByteSize;
	mMeshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mMeshGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)mIndices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mMeshGeo->DrawArgs["triangle"] = submesh;
}*/