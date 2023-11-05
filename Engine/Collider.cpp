#include "Collider.h"
#include "GameObject.h"

using namespace DirectX;

Collider::Collider(GameObject* gameObject) : Component(gameObject)
{

}

Collider::~Collider()
{

}

float Collider::GetDistance(const GameObject& Gameobject2)
{
	float mDistance;
	XMFLOAT3 mCenter2; 
	mCenter.x = mGameObjectPtr->mTransform.vPosition.x;
	mCenter.y = mGameObjectPtr->mTransform.vPosition.y;
	mCenter.z = mGameObjectPtr->mTransform.vPosition.z;

	mCenter2.x = Gameobject2.mTransform.vPosition.x;
	mCenter2.y = Gameobject2.mTransform.vPosition.y;
	mCenter2.z = Gameobject2.mTransform.vPosition.z;
	
	mDistance = sqrtf(((mCenter.x - mCenter2.x) * (mCenter.x - mCenter2.x)) +
		((mCenter.y - mCenter2.y) * (mCenter.y - mCenter2.y)) +
		((mCenter.z - mCenter2.z) * (mCenter.z - mCenter2.z)));

	return mDistance;
}

bool Collider::IsColliding(const GameObject& Gameobject2)
{
	if (mGameObjectPtr.mCmps)
		return true;
	else:
		return false;
}