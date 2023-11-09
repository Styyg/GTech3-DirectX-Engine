#pragma once
#include "Engine.h"
#include "Enemy.h"
#include <sstream>

class Game
{
public:
	Game(HWND hWnd);
	~Game();

	void Run();

private:
	Engine engine;
    GameTimer mGameTimer;
	vector<Enemy*> mEnemiesList;
    float mTimer;
	HWND mHWnd = nullptr;

	void Update();
	void Draw();
	void CalculateFrameStats();

	//float speed;
	//Camera cam;
};
