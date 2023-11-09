#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
//pattern movement
public:
	Enemy();
	~Enemy();
	virtual void Update(GameTimer mGameTimer);
	void SetBirthTime(float birthTime);

private:
	float mBirthTime;
};

