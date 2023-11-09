#pragma once
#include <Windows.h>
#include <unordered_map>

enum KeyState
{
	NONE,
	DOWN,
	PUSH,
	UP
};

class Input
{

public:
	Input(HWND hwnd);
	~Input();
	void Update();
	KeyState GetKeyState(int);
	POINT GetMouseMove();
	void EnableMouseCapture();
	void DisableMouseCapture();
	bool IsCapturingMouse() { return mCaptureMouse; }

private:
	HWND mHWnd = nullptr;
	std::unordered_map<int, KeyState> mKeyStates;
	POINT mLastMousePos;
	POINT mMouseMove;
	bool mCaptureMouse = false;

	void CaptureMousePos();
	void CenterCursor();
};

