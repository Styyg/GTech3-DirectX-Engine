#include "Input.h"
#include <DirectXMath.h>

using namespace DirectX;


Input::Input()
{
    keyStates.emplace(VK_LBUTTON, NONE);
    keyStates.emplace(VK_RBUTTON, NONE);
}

Input::~Input()
{

}

void Input::Update()
{
    for (auto it = keyStates.begin(); it != keyStates.end(); it++)
    {
        // Si appuyé
        if (GetAsyncKeyState(it->first))
        {
            switch (keyStates[it->first])
            {
            case NONE:
                keyStates[it->first] = DOWN;
                break;
            case DOWN:
                keyStates[it->first] = PUSH;
                break;
            case UP:
                keyStates[it->first] = DOWN;
                break;
            default:
                break;
            }
        }
        // si pas appuyé
        else {
            switch (keyStates[it->first])
            {
            case PUSH:
                keyStates[it->first] = UP;
                break;
            case UP:
                keyStates[it->first] = NONE;
                break;
            case DOWN:
                keyStates[it->first] = UP;
                break;
            default:
                break;
            }
        }
    }
}

KeyState Input::GetKeyState(int keycode)
{
    return keyStates[keycode];
}

POINT Input::CaptureMousePosition(HWND hwnd, int mouseX, int mouseY)
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hwnd, &cursorPos);
    mouseX = cursorPos.x;
    mouseY = cursorPos.y;  
    return cursorPos;
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