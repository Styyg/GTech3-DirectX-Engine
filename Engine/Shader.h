#include "d3dUtil.h"

using namespace std;
using namespace Microsoft::WRL;

class Shader
{
public:
	Shader(wstring shaderFile, string entryPoint, string target);
	~Shader();
	
	ComPtr<ID3DBlob> GetShaderByteCode();

private:
	ComPtr<ID3DBlob> mShaderByteCode;
};