#pragma once
#include <DirectXMath.h>
//#include "GameObject.h"
#include "Component.h"

class GameObject;

class Collider : public Component
{
private:
	DirectX::XMFLOAT3 mScale;
	
public:
	DirectX::XMFLOAT3 mCenter;
	DirectX::XMFLOAT3 mRadius;
	Collider(GameObject* gameObject);
	~Collider();
	float GetDistance(const GameObject& Gameobject2);
	//bool IsColliding(GameObject* mGameObjectPtr, const GameObject& Gameobject2);
}; 