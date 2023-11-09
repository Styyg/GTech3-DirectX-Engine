#include "Camera.h"

Camera::Camera() : mPosition(0.0f, 0.0f, -5.0f), mLookAt(0.0f, 0.0f, 0.0f)
{
	XMStoreFloat4x4(&mView, XMMatrixIdentity());
	XMStoreFloat4x4(&mProj, XMMatrixIdentity());
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Camera::SetLookAt(float x, float y, float z)
{
	mLookAt = XMFLOAT3(x, y, z);
}

void Camera::Update()
{

}

XMFLOAT3 Camera::GetPosition() const
{
	return mPosition;
}

XMFLOAT3 Camera::GetLookAt() const
{
	return mLookAt;
}

XMMATRIX Camera::GetViewMatrix()
{
	//XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR target = XMLoadFloat3(&mLookAt);
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
