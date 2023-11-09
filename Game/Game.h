#pragma once
#include "Engine.h"
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
    float mTimer;
	void Update();
	void Draw();

	//float speed;
	//Camera cam;
};
