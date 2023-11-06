#pragma once

#include <iostream>
#include <vector>

#include "Component.h"
#include "Transform.h"

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component component);

	// CB
	void CreateCB(ID3D12Device* pDevice);

	void SetGeo(MeshGeometry* pGeo) { m_pGeo = pGeo; }
	// PSO
	void SetPSO(ID3D12PipelineState* pso);
	ID3D12PipelineState* GetPSO() const;

	void SetupBuffers(ID3D12GraphicsCommandList* commandList);
	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	string mName;
	Transform mTransform;
	vector<Component>mCmps;
	ID3D12PipelineState* m_PSO;
	MeshGeometry* m_pGeo;
public:
	UploadBuffer<ObjectConstants>* mObjectCB = nullptr;
};