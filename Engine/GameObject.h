#pragma once

#include <iostream>
#include <vector>
#include "Component.h"
#include "MeshRenderer.h"
#include "Transform.h"

using namespace std;

enum ComponentType
{
	COLLIDER,
	RIGIDBODY,
	MESH,
	TRIGGER
};

class GameObject
{
private:
	string mName;
	unordered_map<ComponentType,Component>mCmps;
public:
	Transform mTransform;
	GameObject();
	~GameObject();
	void Render();
	void update();
	void AddCollider(Collider collider);
	void AddRigidbody(Rigidbody rigidbody);
	void AddMesh(Mesh mesh);
	//void AddTrigger(Trigger trigger);
};

private:
	string mName;
	Transform mTransform;
	vector<Component> mCmps;
};