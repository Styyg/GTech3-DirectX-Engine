#pragma once
#include <DirectXMath.h>
#include "MathHelper.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void Update();

	XMMATRIX GetViewMatrix(float x, float y, float z);
	XMMATRIX GetProjectionMatrix(float clientWidth, float clientHeight);

private:
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};