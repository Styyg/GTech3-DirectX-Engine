#pragma once
#include <DirectXMath.h>
#include "GameObject.h"

class Collider : public Component
{
private:
	XMFLOAT3 mScale;
	
public:
	XMFLOAT3 mCenter;
	XMFLOAT3 mRadius;
	Collider();
	~Collider();
	float GetDistance(const GameObject& Gameobject2);
	bool IsColliding(GameObject* mGameObjectPtr, const GameObject& Gameobject2);
}; 