#pragma once

#include "d3dUtil.h"

using namespace std;
using namespace Microsoft::WRL;

struct ByteCode {
	ComPtr<ID3DBlob> vsCubeByteCode;
	ComPtr<ID3DBlob> psCubeByteCode;
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	ByteCode CallStack();

	ComPtr<ID3DBlob> CompileShader(wstring shaderFile, string entryPoint, string target);

	ByteCode cubeByteCode;
private:
	ComPtr<ID3DBlob> test;
};