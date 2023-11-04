#pragma once

#include "d3dUtil.h"

struct ByteCode {
	Microsoft::WRL::ComPtr<ID3DBlob> vsCubeByteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> psCubeByteCode;
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	ByteCode CallStack();

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(std::wstring shaderFile, std::string entryPoint, std::string target);
private:
	
};