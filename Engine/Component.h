#pragma once

//#include "GameObject.h"

class GameObject;

class Component
{
public:
	Component(GameObject* gameObject);
	~Component();

	enum Type
	{
		COLLIDER,
		RIGIDBODY,
		MESH,
		TRIGGER
	};

protected:
	GameObject* mGameObjectPtr;
};
