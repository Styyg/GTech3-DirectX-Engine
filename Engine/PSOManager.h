#pragma once

#include "d3dUtil.h"

using namespace std;
using namespace Microsoft::WRL;

class PSOManager {
public:
    PSOManager();
    ~PSOManager();

    ID3D12PipelineState* GetOrCreatePSO(const wstring& psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc, ID3D12Device* device);

private:
    unordered_map<wstring, ComPtr<ID3D12PipelineState>> m_PSOMap;
};
