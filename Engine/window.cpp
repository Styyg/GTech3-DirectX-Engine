#include "window.h"
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Class window
window::windowClass window::windowClass::wndClass;

window::windowClass::windowClass() noexcept : hInst (GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof((wc));
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}


window::windowClass::~windowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

const char* window::windowClass::getName() noexcept
{
	return wndClassName;
}

HINSTANCE window::windowClass::getInstance() noexcept
{
	return wndClass.hInst;
}

window::window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create window & get hWnd
	hWnd = CreateWindow(
		windowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, windowClass::getInstance(), this
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

window::~window()
{
	DestroyWindow(hWnd);
}