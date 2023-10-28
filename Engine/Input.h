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

struct MousePosition {
	int x;
	int y;
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
	// Convenience overrides for handling mouse input.
	//virtual void CaptureMousePosition(HWND hwnd,int mouseX, int mouseY);
	//HWND      mhMainWnd = nullptr; // main window handle


private:
	std::unordered_map<int, KeyState> mKeyStates;
	POINT mLastMousePos;
	POINT mMouseMove;
	//POINT mLastMousePos;
	//float mPhi = XM_PIDIV4;

};

