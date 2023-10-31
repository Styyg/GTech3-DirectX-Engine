#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh() 
{

}

void Mesh::CreateTriangle3D()
{
	mVertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+0.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Blue) })
	};

	mIndices =
	{
		// front face
		0, 1, 2,
		// back face
		0, 2, 1,
	};
}
void Mesh::CreateMesh()
{
	const UINT vbByteSize = (UINT)mVertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)mIndices.size() * sizeof(std::uint16_t);

	mMeshGeo = std::make_unique<MeshGeometry>();
	mMeshGeo->Name = "triGeo";

	mMeshGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), mVertices.data(), vbByteSize, mMeshGeo->VertexBufferUploader);

	mMeshGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), mIndices.data(), ibByteSize, mMeshGeo->IndexBufferUploader);

	mMeshGeo->VertexByteStride = sizeof(Vertex);
	mMeshGeo->VertexBufferByteSize = vbByteSize;
	mMeshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mMeshGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)mIndices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mTriangleGeo->DrawArgs["triangle"] = submesh;
}