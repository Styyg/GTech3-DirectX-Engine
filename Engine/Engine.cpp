#include "Engine.h"
#include <sstream>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
}; 

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

Engine::Engine(HWND hWnd) : mHWnd(hWnd)
{
	InitD3D();
	SynchroProcess();
	SetMSAA();
	CreateCommandAllocator();
	CreateCommandList();
	CreateCommandQueue();
	SwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	DescribeDepthStencilBuffer();
	BuildShadersAndInputLayout();
	BuildConstantBuffers();
	BuildRootSignature();
	BuildTriangleGeometry();
	//BuildPSO();
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
	sd.BufferDesc.Width = GetClientWidth();
	sd.BufferDesc.Height = GetClientHeight();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = mHWnd;
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
	depthStencilDesc.Width = GetClientWidth();
	depthStencilDesc.Height = GetClientHeight();
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
	mViewport.Width = static_cast<float>(GetClientWidth());
	mViewport.Height = static_cast<float>(GetClientHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mCommandList->RSSetViewports(1, &mViewport);

	mScissorRect.left = 0;
	mScissorRect.top = 0;
	mScissorRect.right = GetClientWidth() / 2;
	mScissorRect.bottom = GetClientHeight() / 2;

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

	// config of CBV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 10;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvHeapDesc.NodeMask = 0;
	// creation of DSV desc heap
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&cbvHeapDesc, IID_PPV_ARGS(mCbvHeap.GetAddressOf())));
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

void Engine::BuildShadersAndInputLayout()
{
	ByteCode bc = shaderManager.CallStack();
	mVsByteCode = bc.vsCubeByteCode;
	mPsByteCode = bc.psCubeByteCode;

	mInputLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
}

void Engine::BuildRootSignature()
{

	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	slotRootParameter[0].InitAsConstantBufferView(0);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	// create a root signature with a single slot which points to a
	// descriptor range consisting of a single constant buffer.
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	ThrowIfFailed(mD3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
}

void Engine::BuildConstantBuffers()
{
	//UINT elementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	//ComPtr<ID3D12Resource> mUploadCBuffer;
}

void Engine::BuildTriangleGeometry()
{
	std::array<Vertex, 3> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+0.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Blue) })
	};

	std::array<std::uint16_t, 6> indices =
	{
		// front face
		0, 1, 2,
		// back face
		0, 2, 1,
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mTriangleGeo = std::make_unique<MeshGeometry>();
	mTriangleGeo->Name = "triGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mTriangleGeo->VertexBufferCPU));
	CopyMemory(mTriangleGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mTriangleGeo->IndexBufferCPU));
	CopyMemory(mTriangleGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mTriangleGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mTriangleGeo->VertexBufferUploader);

	mTriangleGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mTriangleGeo->IndexBufferUploader);

	mTriangleGeo->VertexByteStride = sizeof(Vertex);
	mTriangleGeo->VertexBufferByteSize = vbByteSize;
	mTriangleGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mTriangleGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mTriangleGeo->DrawArgs["triangle"] = submesh;
}

void Engine::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mVsByteCode->GetBufferPointer()),
		mVsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mPsByteCode->GetBufferPointer()),
		mPsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(mD3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

LONG Engine::GetClientWidth() 
{
	RECT rect;
	GetWindowRect(mHWnd, &rect);
	return rect.right - rect.left;
}

LONG Engine::GetClientHeight() 
{
	RECT rect;
	GetWindowRect(mHWnd, &rect);
	return rect.bottom - rect.top;
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

void Engine::Update() {

}