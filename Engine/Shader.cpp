#include "Shader.h"

Shader::Shader(wstring shaderFile, string entryPoint, string target)
{
	mShaderByteCode = d3dUtil::CompileShader(shaderFile, nullptr, entryPoint, target);
}

Shader::~Shader()
{

}

ComPtr<ID3DBlob> Shader::GetShaderByteCode()
{
	return mShaderByteCode;
}
