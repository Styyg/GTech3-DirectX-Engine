#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
   
}

ShaderManager::~ShaderManager()
{

}

ByteCode ShaderManager::CallStack()
{  
    ByteCode cubeByteCode;

    cubeByteCode.vsCubeByteCode = CompileShader(L"C:/Users/lprieu/Documents/GitHub/GTech3-DirectX-Engine/Engine/Shaders/cube_shader.hlsl", "VS", "vs_5_0");
    cubeByteCode.psCubeByteCode = CompileShader(L"C:/Users/lprieu/Documents/GitHub/GTech3-DirectX-Engine/Engine/Shaders/cube_shader.hlsl", "PS", "ps_5_0");

    return cubeByteCode;
}

ComPtr<ID3DBlob> ShaderManager::CompileShader(wstring shaderFile, string entryPoint, string target)
{
    ComPtr<ID3DBlob> vertexShaderByteCode = d3dUtil::CompileShader(shaderFile, nullptr, entryPoint, target);

    return vertexShaderByteCode;
}