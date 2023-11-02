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
