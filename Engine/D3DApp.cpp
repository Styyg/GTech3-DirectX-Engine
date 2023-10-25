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
	InitMainWindow();
	InitD3D();
	SynchroProcess();
	SetMSAA();
	CreateCommandAllocator();
	CreateCommandList();
	CreateCommandQueue();
}

D3DApp::~D3DApp()
{

}

void D3DApp::SynchroProcess()
{
	HRESULT hr = mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

	ThrowIfFailed(hr);

	UINT mRtvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	UINT mDsvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	UINT mCbvSrvUavDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void D3DApp::SetMSAA()
{
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(mD3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

	if (msQualityLevels.NumQualityLevels == 0) {
		MessageBox(0, "4x MSAA n'est pas supporté sur votre appareil", "Erreur", MB_OK);

		// ex : 1x MSAA ou pas de MSAA
	}
}

void D3DApp::CreateCommandAllocator()
{
	HRESULT hr = mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));

	ThrowIfFailed(hr);
}

void D3DApp::ConfigSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = mhWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	// Create the swap chain
	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(mDxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		&swapChain)
	);

	// Query the swap chain to the IDXGISwapChain3 interface
	ThrowIfFailed(swapChain.As(&mSwapChain));
}

void D3DApp::CreateCommandList()
{
	HRESULT hr = mD3DDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&mCommandList)
	);
	
	ThrowIfFailed(hr);
}

void D3DApp::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	HRESULT hr = mD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));

	ThrowIfFailed(hr);
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

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	// create window instance
	mhWnd = CreateWindowEx(0, pClassName, "Game Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, mClientWidth, mClientHeight, nullptr, nullptr, mhInst, nullptr);
	if (!mhWnd)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	// show the damn window
	ShowWindow(mhWnd, SW_SHOW);
	//UpdateWindow(mhWnd);

	return true;
}

void D3DApp::InitD3D()
{
	HRESULT hr = D3D12CreateDevice(
		nullptr,                // pAdapter
		D3D_FEATURE_LEVEL_11_0, // Minimum feature level this app can support
		IID_PPV_ARGS(&mD3DDevice)
	);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "Failed to initialize Direct3D", "Error", MB_OK);
	}
}

int D3DApp::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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

void D3DApp::Update() {

}

void D3DApp::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::runtime_error("A Direct3D operation failed with code: " + std::to_string(hr));
	}
}