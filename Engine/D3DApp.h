#pragma once

#include <windows.h>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance);
    virtual ~D3DApp();

    virtual bool Initialize();
    int Run();

private:
    bool InitMainWindow();
    bool InitDirect3D();

    HINSTANCE mhInst = nullptr;
    HWND mhWnd = nullptr;
};

