#pragma once
#include "Mesh.h"
#include "Collider.h"
#include "Rigidbody.h"

class Component
{
private:
	Mesh mMesh;
	Collider mCollider;
	Rigidbody mRigidbody;
public:
	Component();
	~Component();
};

