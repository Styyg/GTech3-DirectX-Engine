#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
    mVertexShaderBlob = nullptr;

    InitShaders();
}

ShaderManager::~ShaderManager()
{
    if (mVertexShaderBlob) mVertexShaderBlob->Release();
}

void ShaderManager::InitShaders()
{
    // Cube Shader
    shaderStruct cubeShader{};

    cubeShader.path = L"Shaders/cube_shader.hlsl";
    cubeShader.vsVersion = "vs_5_0";
    cubeShader.psVersion = "ps_5_0";
    cubeShader.vsEntryPoint = "VS";
    cubeShader.psEntryPoint = "PS";

    cubeShader.shaderBlob = &mVertexShaderBlob;

    CompileShader(cubeShader.path, cubeShader.vsVersion, cubeShader.vsEntryPoint, cubeShader.shaderBlob);
    CompileShader(cubeShader.path, cubeShader.psVersion, cubeShader.psEntryPoint, cubeShader.shaderBlob);

    // Sphere Shader

    // ....
}

void ShaderManager::CompileShader(LPCWSTR path, LPCSTR shaderVersion, LPCSTR entryPoint, ID3DBlob** shaderBlob)
{
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(
        path,
        nullptr,
        nullptr,
        entryPoint,
        shaderVersion,
        0,
        0,
        shaderBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "Shader Compilation Error", MB_OK);
            errorBlob->Release();
        }
        else
        {
            MessageBox(nullptr, L"Failed to compile shader.", L"Error", MB_OK);
        }
    }
}