#include "GeometryGenerator.h"

GeometryGenerator::GeometryGenerator()
{

}

GeometryGenerator::~GeometryGenerator()
{

}

Mesh GeometryGenerator::CreateTriangle3D(float width, float height, float depth)
{
	Mesh mesh;

	float w = 0.5f * width;
	float h = 0.5f * height;
	float d = 0.5f * depth;

	mesh.vertices =
	{
		// front face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Red) }), // up 
		Vertex({ XMFLOAT3(+w, -h, -d), XMFLOAT4(Colors::Red) }), // front right
		Vertex({ XMFLOAT3(-w, -h, -d), XMFLOAT4(Colors::Red) }), // front left
		// left back face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Red) }), // up 
		Vertex({ XMFLOAT3(-w, -h, -d), XMFLOAT4(Colors::Red) }), // front left
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Red) }), // back middle
		// right back face
		Vertex({ XMFLOAT3(0.0f, +h, 0.0f), XMFLOAT4(Colors::Red) }), // up 
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Red) }), // back middle
		Vertex({ XMFLOAT3(+w, -h, -d), XMFLOAT4(Colors::Red) }), // front right
		// bottom face
		Vertex({ XMFLOAT3(+w, -h, -d), XMFLOAT4(Colors::Red) }), // front right
		Vertex({ XMFLOAT3(0.0f, -h, d), XMFLOAT4(Colors::Red) }), // back middle
		Vertex({ XMFLOAT3(-w, -h, -d), XMFLOAT4(Colors::Red) }), // front left
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

Mesh GeometryGenerator::CreateCube(float width, float height, float depth)
{
	Mesh mesh;

	float w = 0.5f * width;
	float h = 0.5f * height;
	float d = 0.5f * depth;

	mesh.vertices =
	{
		// front vertices
		Vertex({ XMFLOAT3(-w, -h, -d), XMFLOAT4(Colors::Coral) }), // bottom left
		Vertex({ XMFLOAT3(-w, +h, -d), XMFLOAT4(Colors::Coral) }), // up left
		Vertex({ XMFLOAT3(+w, +h, -d), XMFLOAT4(Colors::Coral) }), // up right
		Vertex({ XMFLOAT3(+w, -h, -d), XMFLOAT4(Colors::Coral) }), // bottom right
		// back vertices
		Vertex({ XMFLOAT3(-w, -h, +d), XMFLOAT4(Colors::Coral) }), // bottom left
		Vertex({ XMFLOAT3(-w, +h, +d), XMFLOAT4(Colors::Coral) }), // up left
		Vertex({ XMFLOAT3(+w, +h, +d), XMFLOAT4(Colors::Coral) }), // up right
		Vertex({ XMFLOAT3(+w, -h, +d), XMFLOAT4(Colors::Coral) }), // bottom right
	};

	mesh.indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	return mesh;
}