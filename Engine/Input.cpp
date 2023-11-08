#include "Input.h"
#include <DirectXMath.h>

using namespace DirectX;


Input::Input(HWND hWnd)
{
    mHWnd = hWnd;

    //init last mouse pos
    CaptureMousePos();

    mKeyStates.emplace(VK_LBUTTON, NONE);
    mKeyStates.emplace(VK_RBUTTON, NONE);
}

Input::~Input()
{

}

void Input::Update()
{
    CaptureMousePos();

    // key pressed (including mouse buttons)
    for (auto it = mKeyStates.begin(); it != mKeyStates.end(); it++)
    {
        // Si appuyé
        if (GetAsyncKeyState(it->first))
        {
            switch (mKeyStates[it->first])
            {
            case NONE:
                mKeyStates[it->first] = DOWN;
                break;
            case DOWN:
                mKeyStates[it->first] = PUSH;
                break;
            case UP:
                mKeyStates[it->first] = DOWN;
                break;
            default:
                break;
            }
        }
        // si pas appuyé
        else {
            switch (mKeyStates[it->first])
            {
            case PUSH:
                mKeyStates[it->first] = UP;
                break;
            case UP:
                mKeyStates[it->first] = NONE;
                break;
            case DOWN:
                mKeyStates[it->first] = UP;
                break;
            default:
                break;
            }
        }
    }
}

KeyState Input::GetKeyState(int keycode)
{
    return mKeyStates[keycode];
}

void Input::CaptureMousePos()
{
    if (mCaptureMouse)
    {
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(mHWnd, &mousePos);
        CenterCursor();

        mMouseMove.x = mousePos.x - mLastMousePos.x;
        mMouseMove.y = mousePos.y - mLastMousePos.y;
    }
}

void Input::CenterCursor()
{
    POINT p;
    p.x = 800 * 0.5;
    p.y = 600 * 0.5;

    mLastMousePos.x = p.x;
    mLastMousePos.y = p.y;

    ClientToScreen(mHWnd, &p);
    SetCursorPos(p.x, p.y);
}

// return delta of mouse pos in a POINT struct
POINT Input::GetMouseMove()
{
    return mMouseMove;
}

void Input::EnableMouseCapture()
{
    mCaptureMouse = true;
    ShowCursor(false); 
    CenterCursor();
}

void Input::DisableMouseCapture()
{
    mCaptureMouse = false;
    ShowCursor(true);
    mMouseMove.x = 0;
    mMouseMove.y = 0;
}
