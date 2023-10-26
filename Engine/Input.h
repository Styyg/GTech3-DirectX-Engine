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
protected:
	// Convenience overrides for handling mouse input.
	//virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	//virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	//virtual void OnMouseMove(WPARAM btnState, int x, int y) { }
	//HWND      mhMainWnd = nullptr; // main window handle


private:
	std::unordered_map<int, KeyState> keyStates;
	//POINT mLastMousePos;
	//float mPhi = XM_PIDIV4;

};

