#pragma once
#include <Windows.h>
#include <unordered_map>

enum KeyState
{
	NONE,
	UP,
	DOWN,
	PUSH
};

class Input
{

public:
	Input();
	~Input();
	void Update();
	KeyState GetKeyState(int);
	void CaptureMousePos(HWND hwnd);
	POINT GetMouseMove();
protected:


private:
	std::unordered_map<int, KeyState> mKeyStates;
	POINT mLastMousePos;
	POINT mMouseMove;
};

