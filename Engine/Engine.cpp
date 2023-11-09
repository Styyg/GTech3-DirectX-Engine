#include "Engine.h"
#include "GameObject.h"
#include <sstream>

#include "Manager.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

Engine::Engine(HWND hWnd) : mHWnd(hWnd), input(hWnd)
{
	InitD3D();
	SynchroProcess();
	SetMSAA();
	CreateCommandObjects();
	CreateSwapChain();
	CreateDescriptorHeaps();

	OnResize();

	//RenderTargetView();
	//DescribeDepthStencilBuffer();
	BuildRootSignature();

	//ResetCommandList();
	BuildTriangleGeometry();
	//CloseCommandeList();
	//ExecuteCommandList();
	Flush();
	// aditionnal free upload buffer

	BuildAllGameObjects(); 
	
}

Engine::~Engine()
{

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

void Engine::OnResize()
{
	assert(mD3DDevice);
	assert(mSwapChain);
	assert(mCommandAllocator);

	// Flush before changing any resources.
	FlushCommandQueue();

	ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < mSwapChainBufferCount; ++i)
		mSwapChainBuffer[i].Reset();
	mDepthStencilBuffer.Reset();

	// Resize the swap chain.
	ThrowIfFailed(mSwapChain->ResizeBuffers(
		mSwapChainBufferCount,
		mClientWidth, mClientHeight,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mCurrentBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < mSwapChainBufferCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		mD3DDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	ThrowIfFailed(mD3DDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	// Execute the resize commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommandQueue();

	// Update the viewport transform to cover the client area.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mClientWidth);
	mViewport.Height = static_cast<float>(mClientHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };
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
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(mD3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

	if (msQualityLevels.NumQualityLevels == 0) {
		MessageBox(0, L"4x MSAA n'est pas supporté sur votre appareil", L"Erreur", MB_OK);

		// ex : 1x MSAA ou pas de MSAA
	}
}

void Engine::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	mSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;	
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = mSwapChainBufferCount;
	sd.BufferDesc.Width = GetClientWidth();
	sd.BufferDesc.Height = GetClientHeight();
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.OutputWindow = mHWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(mDxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		&mSwapChain)
	);

	// Query the swap chain to the IDXGISwapChain3 interface
	//ThrowIfFailed(swapChain.As(&mSwapChain));
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

void Engine::CreateCommandObjects()
{
	// Command Queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(mD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	// Command Allocator
	ThrowIfFailed(mD3DDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(&mCommandAllocator)));

	// Command List
	ThrowIfFailed(mD3DDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), // Associated command allocator
		nullptr,				 // Initial PipelineStateObject
		IID_PPV_ARGS(&mCommandList)));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	mCommandList->Close();
}

void Engine::CreateDescriptorHeaps()
{
	// config of RTV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = mSwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

	// config of DSV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));

	// config of CBV desc heap
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1000;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(
		&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
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

void Engine::ResetCommandList()
{
	//mCommandList->Reset(mCommandAllocator.Get(), nullptr);
	mCommandList->Reset(mCommandAllocator.Get(), mPSO.Get());
}

void Engine::CloseCommandeList()
{
	mCommandList->Close();
}

void Engine::ExecuteCommandList()
{
	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void Engine::Flush()
{
	mFenceValue++;
	mCommandQueue->Signal(mFence.Get(), mFenceValue);

	if (mFence->GetCompletedValue() < mFenceValue)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (eventHandle == nullptr)
		{
			// Handle error
		}

		// Attendre jusqu'à ce que le fence ait été traversé
		mFence->SetEventOnCompletion(mFenceValue, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

//void Engine::BuildPSO()
void Engine::BuildAllGameObjects()
{
	// Shader
	vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	Shader& myVertexShader = shaderManager.AddShader(L"MyVertexShader", L"../Engine/Shaders/cube_shader.hlsl", "VS", "vs_5_0");
	Shader& myPixelShader = shaderManager.AddShader(L"MyPixelShader", L"../Engine/Shaders/cube_shader.hlsl", "PS", "ps_5_0");

	Shader* vertexShader = shaderManager.GetShader(L"MyVertexShader");
	Shader* pixelShader = shaderManager.GetShader(L"MyPixelShader");

	ComPtr<ID3DBlob> vertexShaderByteCode = vertexShader->GetShaderByteCode();
	ComPtr<ID3DBlob> pixelShaderByteCode = pixelShader->GetShaderByteCode();

	// Shaders
	mBasePsoDesc.VS = { reinterpret_cast<BYTE*>(vertexShaderByteCode->GetBufferPointer()), vertexShaderByteCode->GetBufferSize() };
	mBasePsoDesc.PS = { reinterpret_cast<BYTE*>(pixelShaderByteCode->GetBufferPointer()), pixelShaderByteCode->GetBufferSize() };

	// États par défaut pour le reste du PSO
	mBasePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	mBasePsoDesc.SampleMask = UINT_MAX;
	mBasePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	mBasePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// Formats de rendu
	mBasePsoDesc.RTVFormats[0] = mBackBufferFormat;
	mBasePsoDesc.DSVFormat = mDepthStencilFormat;
	mBasePsoDesc.SampleDesc.Count = 1;
	mBasePsoDesc.SampleDesc.Quality = 0;

	// Nombre de RenderTargets
	mBasePsoDesc.NumRenderTargets = 1;

	// Primitive Topology
	mBasePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// Input Layout
	mBasePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };

	// Root Signature
	mBasePsoDesc.pRootSignature = mRootSignature.Get();

	Manager* mgr = Manager::GetInstance();

	GameObject* cube = new GameObject;
	mgr->AddGameObject(cube);

	GameObject* obj2 = new GameObject;
	mgr->AddGameObject(obj2);

	ID3D12PipelineState* objPSO = mPsoManager.GetOrCreatePSO(L"ObjPSO", mBasePsoDesc, mD3DDevice.Get());
	cube->SetPSO(objPSO);
	cube->CreateCB(mD3DDevice.Get());
	cube->SetGeo(mTriangleGeo.get());

	obj2->SetPSO(objPSO);
	obj2->CreateCB(mD3DDevice.Get());
	obj2->SetGeo(mTriangleGeo.get());
}

void Engine::DrawAllGameObjects()
{
	Manager* mgr = Manager::GetInstance();
	list<GameObject*>& gameObjects = mgr->GetGameObjects();
	
	float i = 0.1f;
	for (GameObject* obj : gameObjects) {
		// Utilisez le PSO spécifique à l'objet
		ID3D12PipelineState* pso = obj->GetPSO();
		if (pso != nullptr) {
			mCommandList->SetPipelineState(pso);
		}
	
		mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
		mCommandList->SetGraphicsRootConstantBufferView(0, obj->mObjectCB->Resource()->GetGPUVirtualAddress());

		// Assumer que chaque GameObject sait comment configurer ses propres buffers
		obj->SetupBuffers(mCommandList.Get());
	
		// Assumer que chaque GameObject sait comment dessiner lui-même
		obj->Draw(mCommandList.Get());
	}
}

void Engine::BuildRootSignature()
{
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	 slotRootParameter[0].InitAsConstantBufferView(0);

	 //CD3DX12_DESCRIPTOR_RANGE cbvTable1;
	 //cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	 //slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable1);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	// create a root signature with a single slot which points to a
	// descriptor range consisting of a single constant buffer.
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	ThrowIfFailed(mD3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
}

void Engine::BuildTriangleGeometry()
{
	ResetCommandList();

	GeometryGenerator geoGen;
	Mesh triangle = geoGen.CreateTriangle3D(1.0f, 1.0f, 1.0f);

	const UINT vbByteSize = (UINT)triangle.vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)triangle.indices.size() * sizeof(std::uint16_t);

	mTriangleGeo = make_unique<MeshGeometry>();
	mTriangleGeo->Name = "triGeo";

	mTriangleGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), triangle.vertices.data(), vbByteSize, mTriangleGeo->VertexBufferUploader);

	mTriangleGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), triangle.indices.data(), ibByteSize, mTriangleGeo->IndexBufferUploader);

	mTriangleGeo->VertexByteStride = sizeof(Vertex);
	mTriangleGeo->VertexBufferByteSize = vbByteSize;
	mTriangleGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mTriangleGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)triangle.indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mTriangleGeo->DrawArgs["triangle"] = submesh;

	mTriangleGeo->indexCount = submesh.IndexCount;

	CloseCommandeList();
	ExecuteCommandList();
}

void Engine::CreateCube(GameObject* gameObject,float width, float height, float depth, float x, float y, float z)
{
	ResetCommandList();

	GeometryGenerator geoGen;
	Mesh cube = geoGen.CreateCube(width, height, depth);

	const UINT vbByteSize = (UINT)cube.vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)cube.indices.size() * sizeof(std::uint16_t);

	MeshGeometry* geo = new MeshGeometry;
	geo->Name = "geo";

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), cube.vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(mD3DDevice.Get(),
		mCommandList.Get(), cube.indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)cube.indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	geo->DrawArgs["cube"] = submesh;

	geo->indexCount = submesh.IndexCount;

	CloseCommandeList();
	ExecuteCommandList();

	Manager* mgr = Manager::GetInstance();
	mgr->AddGameObject(gameObject);

	ID3D12PipelineState* objPSO = mPsoManager.GetOrCreatePSO(L"ObjPSO", mBasePsoDesc, mD3DDevice.Get());
	gameObject->SetPSO(objPSO);
	gameObject->CreateCB(mD3DDevice.Get());
	gameObject->SetGeo(geo);
	gameObject->mTransform.SetPosition(x, y, z);
}

void Engine::InitD3D()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
{
	ComPtr<ID3D12Debug> debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
}
#endif

	// Create Factory
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

void Engine::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	mCurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));

	// Wait until the GPU has completed commands up to this fence point.
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void Engine::Update(GameTimer gt)
{
	Manager* mgr = Manager::GetInstance();
	mgr->Update(gt);
	Camera camera;
	camera.Update();
	input.Update();

	//// temporary inputs to move the camera around the center
	if (input.GetKeyState('Z')) mPhi += 1.0f * gt.DeltaTime();
	if (input.GetKeyState('S')) mPhi -= 1.0f * gt.DeltaTime();
	if (input.GetKeyState('Q')) mTheta += 1.0f * gt.DeltaTime();
	if (input.GetKeyState('D')) mTheta -= 1.0f * gt.DeltaTime();
	if (input.GetKeyState('A')) mRadius += 1.0f * gt.DeltaTime();
	if (input.GetKeyState('E')) mRadius -= 1.0f * gt.DeltaTime();

	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	mView = camera.GetViewMatrix(x, y, z);
	mProj = camera.GetProjectionMatrix(800, 600);

	list<GameObject*>& gameObjects = mgr->GetGameObjects();

	for (GameObject* obj : gameObjects) {
		ObjectConstants objConstants;
		//obj->mTransform.RotateYaw(10.0f * gt.TotalTime());
		//obj->mTransform.RotateYaw(10.0f);

		//XMFLOAT4X4 world = obj->mTransform.mWorldMatrix;

		XMMATRIX world = XMLoadFloat4x4(&obj->mTransform.mWorldMatrix);
		XMMATRIX worldViewProj = world * mView * mProj;

		//XMStoreFloat4x4(&mWorldViewProj, XMMatrixTranspose(worldViewProj));
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
		//objConstants.WorldViewProj = mWorldViewProj;
		obj->mObjectCB->CopyData(0, objConstants);
	}



	//// Build the view matrix.
	//XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	//XMVECTOR target = XMVectorZero();
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	//XMStoreFloat4x4(&mView, view);

	//XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, 800/600.0f, 0.5f, 1000.0f);

	// Mise à jour de la matrice de vue et de projection dans le shader
	//XMMATRIX world = XMLoadFloat4x4(&mWorld);
	//XMMATRIX worldViewProj = world * view * proj;
	//XMStoreFloat4x4(&mWorldViewProj, XMMatrixTranspose(worldViewProj));

	//ObjectConstants objConstants;
	//XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	//mObjectCB->CopyData(0, objConstants);
}

void Engine::Draw()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	HRESULT hRes = mCommandAllocator->Reset();

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.

	ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	ID3D12DescriptorHeap* descriptorHeaps[] = {mCbvHeap.Get()};
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	DrawAllGameObjects();
	
	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();

	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mSwapChainBufferCount;
}