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
	HWND mHWnd = nullptr;

	void Update();
	void Draw();
	void CalculateFrameStats();
};
