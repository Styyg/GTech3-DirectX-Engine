#pragma once
#include "GameTimer.h"
#include "Transform.h"
#include "Input.h"
//#include "Collider.h"


class Entity
{
private:
	Transform* mTransform;
public:
	Entity();
	~Entity();

};

class Game
{
	float mTimer = mGameTimer.TotalTime();
	GameTimer mGameTimer;
	void Update();
};

