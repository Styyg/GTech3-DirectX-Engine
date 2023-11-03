#pragma once
#include "Mesh.h"
#include "Collider.h"
#include "Rigidbody.h"

class Component
{
protected:
	GameObject* mGameObjectPtr;
public:
	Component(GameObject* gameObject);
	~Component();
};

