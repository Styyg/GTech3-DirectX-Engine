#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

private:
	Mesh mesh;
	//Shader shader;
	//Constant buffer
};

