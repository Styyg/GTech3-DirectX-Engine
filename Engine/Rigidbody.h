#pragma once
#include "Component.h"

class GameObject;

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* gameObject);;
};

