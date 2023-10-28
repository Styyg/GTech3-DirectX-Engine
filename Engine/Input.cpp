#include "Input.h"
#include <DirectXMath.h>

using namespace DirectX;


Input::Input()
{
    mKeyStates.emplace(VK_LBUTTON, NONE);
    mKeyStates.emplace(VK_RBUTTON, NONE);
}

Input::~Input()
{

}

void Input::Update()
{
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

void Input::CaptureMousePos(HWND hwnd)
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(hwnd, &mousePos);
    mMouseMove.x = mousePos.x - mLastMousePos.x;
    mMouseMove.y = mousePos.y - mLastMousePos.y;
    mLastMousePos = mousePos;
}

POINT Input::GetMouseMove()
{
    return mMouseMove;
}

//void Input::OnMouseMove(WPARAM btnState, int x, int y)
//{
//    if ((btnState & MK_LBUTTON) != 0)
//    {
//        // Make each pixel correspond to a quarter of a degree.
//        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
//        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
//
//        // Update angles based on input to orbit camera around box.
//        mTheta += dx;
//        mPhi += dy;
//
//        // Restrict the angle mPhi.
//        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
//    }
//    else if ((btnState & MK_RBUTTON) != 0)
//    {
//        // Make each pixel correspond to 0.005 unit in the scene.
//        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
//        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);
//
//        // Update the camera radius based on input.
//        mRadius += dx - dy;
//
//        // Restrict the radius.
//        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
//    }
//
//    mLastMousePos.x = x;
//    mLastMousePos.y = y;
//}