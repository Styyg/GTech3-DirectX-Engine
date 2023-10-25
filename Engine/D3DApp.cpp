#include "D3DApp.h"
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "Ta mère");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "Une fenetre");
		}
		break;
	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowText(hWnd, oss.str().c_str());
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)	: mhInst(hInstance)
{

}

D3DApp::~D3DApp()
{

}

bool D3DApp::Initialize()
{
	if (!InitMainWindow())
		return false;

	if (!InitDirect3D())
		return false;

	return true;
}

bool D3DApp::InitMainWindow()
{
	const auto pClassName = "GameWnd";
	// register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// create window instance
	mhWnd = CreateWindowEx(0, pClassName, "Game Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, 640, 480, nullptr, nullptr, mhInst, nullptr);

	// show the damn window
	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);

	return true;
}

bool D3DApp::InitDirect3D()
{
	return true;
}

int D3DApp::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{

		}
	}

	return (int)msg.wParam;;
}