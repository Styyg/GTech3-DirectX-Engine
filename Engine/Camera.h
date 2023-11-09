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

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLookAt() const;
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix(float clientWidth, float clientHeight);

private:
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};