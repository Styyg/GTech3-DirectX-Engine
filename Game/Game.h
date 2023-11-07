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

	void Update();
	void Draw();
};