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
	void Update();
	void Draw();

};
