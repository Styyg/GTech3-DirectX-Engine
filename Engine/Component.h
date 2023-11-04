#pragma once
#include "Mesh.h"
#include "Collider.h"
#include "Rigidbody.h"

class GameObject;

class Component
{
protected:
	GameObject* mGameObjectPtr;
public:
	Component(GameObject* gameObject);
	~Component();

private:
	GameObject* mGameObjectPtr;
};
