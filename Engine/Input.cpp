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
        // Si appuy�
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
        // si pas appuy�
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
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(mHWnd, &mousePos);
    mMouseMove.x = mousePos.x - mLastMousePos.x;
    mMouseMove.y = mousePos.y - mLastMousePos.y;
    mLastMousePos = mousePos;
}

// return delta of mouse pos in a POINT struct
POINT Input::GetMouseMove()
{
    return mMouseMove;
}