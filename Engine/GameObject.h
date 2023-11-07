#pragma once
#include <iostream>
#include <unordered_map>
#include "Transform.h"
//#include "Component.h"
#include "Collider.h"
#include "Rigidbody.h" 
#include "Mesh.h" 

#include "Engine.h"

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

	//

	void Render();
	void Update();
	void AddCollider(Collider* collider);
	void AddRigidbody(Rigidbody* rigidbody);
	Collider* GetCollider();
	//void AddMesh(Mesh mesh);
	//void AddTrigger(Trigger trigger);

	Transform mTransform;

	UploadBuffer<ObjectConstants>* mObjectCB = nullptr;
	UploadBuffer<PassConstants>* mPassConstants = nullptr;

private:
	ID3D12PipelineState* m_PSO;
	MeshGeometry* m_pGeo;

	std::string mName;
	std::unordered_map<Component::Type, Component*> mCmps;
};