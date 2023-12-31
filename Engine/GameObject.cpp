#include "GameObject.h"
//#include "Collider.h"

using namespace DirectX;
using namespace Microsoft::WRL;

GameObject::GameObject(): m_PSO(nullptr)
{
    
}

GameObject::~GameObject()
{
    // TODO: delete component mCmps
    for (auto& pair : mCmps)
    {
        delete pair.second;
    }
}

void GameObject::AddCollider()
{
	mCmps.emplace(Component::COLLIDER, new Collider(this));
}

void GameObject::AddRigidbody(Rigidbody* rigidbody)
{
	mCmps.emplace(Component::RIGIDBODY, rigidbody);
}


Collider* GameObject::GetCollider() {
    // Utilise la fonction find pour chercher le composant Collider dans mCmps.
    auto it = mCmps.find(Component::COLLIDER); 

    // V�rifie si le composant Collider est trouv�.
    if (it != mCmps.end()) {
        // Si le composant Collider est trouv�, renvoie un pointeur vers celui-ci.
        //return (Collider*)c;
        return reinterpret_cast<Collider*>(it->second);
    }
    else {
        // Le composant Collider n'est pas trouv�, renvoie nullptr.
        return nullptr;
    }
}

bool GameObject::HasCollider() {
    // Utilise la fonction find pour chercher le composant Collider dans mCmps.
    auto it = mCmps.find(Component::COLLIDER);

    // V�rifie si le composant Collider est trouv�.
    if (it != mCmps.end()) {
        // Si le composant Collider est trouv�, renvoie true.
        //return (Collider*)c;
        return true;
    }
    else {
        // Le composant Collider n'est pas trouv�, renvoie false.
        return false;
    }
}

void GameObject::Kill()
{
    mToDestroy = true;
}

//void GameObject::AddMesh(Mesh mesh)
//{
//	mCmps.emplace(Component::MESH, mesh);
//}
/*
void GameObject::AddTrigger(Trigger trigger)
{
	mCmps.emplace(TRIGGER, trigger);

}*/

void GameObject::Render()
{

}

void GameObject::Update(GameTimer gt)
{

}

void GameObject::CreateCB(ID3D12Device* pDevice)
{
    mObjectCB = new UploadBuffer<ObjectConstants>(pDevice, 1, true);
    mPassConstants = new UploadBuffer<PassConstants>(pDevice, 1, true);
    
    //D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();

    //D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    //cbvDesc.BufferLocation = cbAddress;
    //cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    //pDevice->CreateConstantBufferView(&cbvDesc, CbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void GameObject::SetPSO(ID3D12PipelineState* pso) {
    m_PSO = pso;
}

ID3D12PipelineState* GameObject::GetPSO() const {
    return m_PSO;
}

void GameObject::SetupBuffers(ID3D12GraphicsCommandList* commandList)
{
    commandList->IASetVertexBuffers(0, 1, &m_pGeo->VertexBufferView());
    commandList->IASetIndexBuffer(&m_pGeo->IndexBufferView());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GameObject::Draw(ID3D12GraphicsCommandList* commandList)
{
    commandList->DrawIndexedInstanced(m_pGeo->indexCount, 1, 0, 0, 0);
}