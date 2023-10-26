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
    shaderStruct cubeShader;

    cubeShader.path = L"Shaders/Shader.hlsl";
    cubeShader.shaderVersion = "vs_5_0";
    cubeShader.entryPoint = "main";
    cubeShader.shaderBlob = &mVertexShaderBlob;

    CompileShader(cubeShader.path, cubeShader.shaderVersion, cubeShader.entryPoint, cubeShader.shaderBlob);

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