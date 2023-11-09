#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
//pattern movement
public:
	Enemy(float birthTime);
	~Enemy();
	virtual void Update(GameTimer mGameTimer);

private:
	float mBirthTime;
};

