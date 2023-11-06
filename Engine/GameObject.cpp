#include "GameObject.h"

GameObject::GameObject(): m_PSO(nullptr)
{
    
}

GameObject::~GameObject()
{

}

void GameObject::AddComponent(Component component)
{
	mCmps.push_back(component);
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