#include "GameObject.h"
//#include "Collider.h"

GameObject::GameObject(): m_PSO(nullptr)
{
    
}

GameObject::~GameObject()
{

}

void GameObject::AddCollider(Collider* collider)
{
	mCmps.emplace(Component::COLLIDER, collider);

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

void GameObject::update()
{
    
}

void GameObject::CreateCB(ID3D12Device* pDevice)
{
    mObjectCB = new UploadBuffer<ObjectConstants>(pDevice, 1, true);
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