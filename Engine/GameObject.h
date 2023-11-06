#pragma once
#include <iostream>
#include <unordered_map>
#include "Transform.h"
//#include "Component.h"
#include "Collider.h"
#include "Rigidbody.h" 
#include "Mesh.h" 

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
	ID3D12PipelineState* m_PSO;
	MeshGeometry* m_pGeo;
public:
	UploadBuffer<ObjectConstants>* mObjectCB = nullptr;
	std::string mName;
	std::unordered_map<Component::Type, Component*> mCmps;

	Transform mTransform;
	GameObject();
	~GameObject();
	void Render();
	void update();
	void AddCollider(Collider* collider);
	void AddRigidbody(Rigidbody* rigidbody);
	Collider* GetCollider();
	//void AddMesh(Mesh mesh);
	//void AddTrigger(Trigger trigger);
};