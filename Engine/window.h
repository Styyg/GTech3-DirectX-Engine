#include <Windows.h>
#include <iostream>


class window
{
private:
	// singleton manages registration/cleanup of window class
	class windowClass
	{
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;
	private:
		windowClass() noexcept;
		~windowClass();
		windowClass(const windowClass&) = delete;
		windowClass& operator=(const windowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Engine";
		static windowClass wndClass;
		HINSTANCE hInst;
	};
public:
	window(int width, int height, const char* name);
	~window();
	window(const window&) = delete;
	window& operator=(const window&) = delete;
private:
	int width;
	int height;
	HWND hWnd;
};