#include "PSOManager.h"

PSOManager::PSOManager()
{
}

PSOManager::~PSOManager()
{
}

ID3D12PipelineState* PSOManager::GetOrCreatePSO(const wstring& psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc, ID3D12Device* device) {
    auto it = m_PSOMap.find(psoName);
    if (it != m_PSOMap.end()) {
        return it->second.Get();
    }
    else {
        ComPtr<ID3D12PipelineState> pso;
        ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso)));
        m_PSOMap[psoName] = pso;
        return pso.Detach();
    }
}