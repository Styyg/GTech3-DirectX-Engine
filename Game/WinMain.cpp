#include "Engine.h"
#include "Game.h"
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	//case WM_KEYDOWN:
	//	if (wParam == 'F')
	//	{
	//		SetWindowText(hWnd, L"Hehe");
	//	}
	//	break;
	//case WM_KEYUP:
	//	if (wParam == 'F')
	//	{
	//		SetWindowText(hWnd, L"Game Window");
	//	}
	//	break;
	//case WM_LBUTTONDOWN:
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	std::wstringstream oss;
	//	oss << "(" << pt.x << "," << pt.y << ")";
	//	SetWindowText(hWnd, oss.str().c_str());
	//	break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const auto pClassName = L"GameWnd";    
	int clientWidth = 800;
	int clientHeight = 600;

	// register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// create window instance
	HWND hWnd = CreateWindowEx(0, pClassName, L"Game Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, clientWidth,clientHeight, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// show the damn window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	try
	{
		Game game(hWnd);

		// Start the game loop
		game.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}