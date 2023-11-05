#pragma once

#include <iostream>
#include <unordered_map>
#include "Transform.h"
//#include "Component.h"
#include "Collider.h"
#include "Rigidbody.h" 
#include "Mesh.h" 

class GameObject
{
private:
	std::string mName;
	std::unordered_map<Component::Type, Component> mCmps;
public:
	Transform mTransform;
	GameObject();
	~GameObject();
	void Render();
	void update();
	void AddCollider(Collider collider);
	void AddRigidbody(Rigidbody rigidbody);
	Collider* GetCollider();
	//void AddMesh(Mesh mesh);
	//void AddTrigger(Trigger trigger);
};