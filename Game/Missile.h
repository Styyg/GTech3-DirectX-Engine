#pragma once

#include "GameObject.h"

class Missile : public GameObject
{
public:
	Missile(Camera* cam);
	~Missile();

	void Update(GameTimer gt);
	void SetBirthTime(float birthTime);

	//XMFLOAT3 GetPosition() const;
	//void SetPosition(const XMFLOAT3& newPos);

	//XMFLOAT3 GetDirection() const;
	//void SetDirection(const XMFLOAT3& newDir);

	//float GetVelocity() const;
	//void SetVelocity(float newVelocity);

private:
	float mBirthTime;
	float mLifeSpan = 5;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	float velocity;
};