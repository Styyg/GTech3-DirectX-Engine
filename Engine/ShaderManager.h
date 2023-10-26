#pragma once

#include <d3dcompiler.h>

struct shaderStruct {
	LPCWSTR path;
	LPCSTR shaderVersion;
	LPCSTR entryPoint;
	ID3DBlob** shaderBlob;
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void InitShaders();
	void CompileShader(LPCWSTR path, LPCSTR shaderVersion, LPCSTR entryPoint, ID3DBlob** shaderBlob);

private:
	ID3DBlob* mVertexShaderBlob;
};