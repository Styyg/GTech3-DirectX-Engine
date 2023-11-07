#include "Camera.h"

Camera::Camera()
{
	XMStoreFloat4x4(&mView, XMMatrixIdentity());
	XMStoreFloat4x4(&mProj, XMMatrixIdentity());
}

Camera::~Camera()
{
}

void Camera::Update()
{

}

XMMATRIX Camera::GetViewMatrix(float x, float y, float z)
{
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
	
	return view;
}

XMMATRIX Camera::GetProjectionMatrix(float clientWidth, float clientHeight)
{
	float aspectRatio = clientWidth/ clientHeight;
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, aspectRatio, 0.5f, 1000.0f);
	XMStoreFloat4x4(&mProj, proj);

	return proj;
}
