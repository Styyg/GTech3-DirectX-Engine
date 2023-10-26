#include "Engine.h"
#include <sstream>

using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"Ta mère");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"Une fenetre");
		}
		break;
	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lParam);
		std::wstringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowText(hWnd, oss.str().c_str());
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

Engine::Engine(HINSTANCE hInstance) : mhInst(hInstance)
{
	InitMainWindow();
	InitD3D();
	SynchroProcess();
	SetMSAA();
	CreateCommandAllocator();
	CreateCommandList();
	CreateCommandQueue();
	SwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	DescribeDepthStencilBuffer();
	BuildInputLayout();
}

Engine::~Engine()
{

}

void Engine::SynchroProcess()
{
	HRESULT hr = mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

	ThrowIfFailed(hr);

	mRtvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Engine::SetMSAA()
{
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(mD3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

	if (msQualityLevels.NumQualityLevels == 0) {
		MessageBox(0, L"4x MSAA n'est pas supporté sur votre appareil", L"Erreur", MB_OK);

		// ex : 1x MSAA ou pas de MSAA
	}
}

void Engine::CreateCommandAllocator()
{
	HRESULT hr = mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));

	ThrowIfFailed(hr);
}

void Engine::SwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd;
	
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = mSwapChainBufferCount;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = mhWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	// Create Factory
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)));

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

void Engine::RenderTargetView()
{
	ComPtr<ID3D12Resource> backBuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	mD3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

	ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < mSwapChainBufferCount; i++)
	{
		// Get the ith buffer in the swap chain.
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));

		// Create an RTV to it.
		mD3DDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

		// Next entry in heap.
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}
}

void Engine::DescribeDepthStencilBuffer()
{
	D3D12_RESOURCE_DESC depthStencilDesc = {};

	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	// 24-bit depth, 8-bit stencil
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE depthStencilClearValue = {};
	depthStencilClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilClearValue.DepthStencil.Depth = 1.0f;
	depthStencilClearValue.DepthStencil.Stencil = 0;

	// Create the depth/stencil buffer.
	mD3DDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthStencilClearValue,
		IID_PPV_ARGS(&mDepthStencilBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	mD3DDevice->CreateDepthStencilView(
		mDepthStencilBuffer.Get(),
		&dsvDesc,
		mDsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void Engine::SetupGraphicsPipeline()
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(mClientWidth);
	mViewport.Height = static_cast<float>(mClientHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mCommandList->RSSetViewports(1, &mViewport);

	mScissorRect.left = 0;
	mScissorRect.top = 0;
	mScissorRect.right = mClientWidth / 2;
	mScissorRect.bottom = mClientHeight / 2;

	mCommandList->RSSetScissorRects(1, &mScissorRect);
}

void Engine::CreateCommandList()
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

void Engine::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	HRESULT hr = mD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));

	ThrowIfFailed(hr);
}

void Engine::CreateRtvAndDsvDescriptorHeaps()
{
	// config of RTV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = mSwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	// creation of RTV desc heap
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

	// config of DSV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	// creation of DSV desc heap
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
}

ID3D12Resource* Engine::CurrentBackBuffer()const
{
	return mSwapChainBuffer[mCurrentBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE Engine::CurrentBackBufferView()const
{
	// CD3DX12 constructor to offset to the RTV of the current back buffer.
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),// handle start
		mCurrentBackBuffer, // index to offset
		mRtvDescriptorSize); // byte size of descriptor
}

D3D12_CPU_DESCRIPTOR_HANDLE Engine::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void Engine::BuildInputLayout()
{
	mInputLayoutDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
}

bool Engine::InitMainWindow()
{
	const auto pClassName = L"GameWnd";
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
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// create window instance
	mhWnd = CreateWindowEx(0, pClassName, L"Game Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, mClientWidth, mClientHeight, nullptr, nullptr, mhInst, nullptr);
	if (!mhWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// show the damn window
	ShowWindow(mhWnd, SW_SHOW);
	//UpdateWindow(mhWnd);

	return true;
}

void Engine::InitD3D()
{
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)));

	HRESULT hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&mD3DDevice)
	);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to initialize Direct3D", L"Error", MB_OK);
	}
}

int Engine::Run()
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
			Update();
		}
	}

	return (int)msg.wParam;;
}

void Engine::Update() {

}

//void Engine::ThrowIfFailed(HRESULT hr)
//{
//	if (FAILED(hr))
//	{
//		throw std::runtime_error("A Direct3D operation failed with code: " + std::to_string(hr));
//	}
//}