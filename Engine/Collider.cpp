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

bool Collider::IsColliding( GameObject& Gameobject2)
{
	// dist - rayonA - rayonB
	float mCollision= mDistance - 1 * 0.5 - 1 * 0.5;
	bool hasCollider = mGameObjectPtr->HasCollider();
	bool hasCollider2 = Gameobject2.HasCollider(); 
	if (hasCollider && hasCollider2 != false)
	{
		if (mCollision <= 0)
			return true;
		else
			return false;
	}	
	else
	{
		return false;
	}
}