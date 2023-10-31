#include "Transform.h"

Transform::Transform() : vPosition(0.0f, 0.0f, 0.0f), vScale(1.0f, 1.0f, 1.0f)
{
    // Constructor: Initialize the transformation with default values
    // Initialize the vectors for the local axes (vRight, vUp, vForward)
    InitializeIdentity();
    vRight.x = 1;
    vRight.y = 0;
    vRight.z = 0;
    vUp.x = 0;
    vUp.y = 1;
    vUp.z = 0;
    vForward.x = 0;
    vForward.y = 0;
    vForward.z = 1;
}

void Transform::InitializeIdentity()
{
    // Initialize the transformation matrices as identity matrices
    // mScale, mRotation, mPosition, and mWorldMatrix represent scaling, rotation, position, 
    // and the combined transformation, respectively
    XMStoreFloat4x4(&mScale, XMMatrixIdentity());
    XMStoreFloat4x4(&mRotation, XMMatrixIdentity());
    XMStoreFloat4x4(&mPosition, XMMatrixIdentity());
    XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity());
}

void Transform::FromMatrix(XMMATRIX* pMat)// useless for now
{
    // update individual scale, rotation, and position matrices
    // qRotation represents the rotation quaternion
    // vScale represents the scale vector
    // vPosition represents the translation vector
    XMStoreFloat4x4(&mScale, XMMatrixScaling(vScale.x, vScale.y, vScale.z));
    XMStoreFloat4x4(&mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation)));
    XMStoreFloat4x4(&mPosition, XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z));
}

void Transform::UpdateRotationFromVectors()
{
    // Update rotation based on provided forward, right, and up vectors
    XMVECTOR newForward = XMLoadFloat3(&vForward);
    XMVECTOR newRight = XMLoadFloat3(&vRight);
    XMVECTOR newUp = XMLoadFloat3(&vUp);

    // Calculate the rotation quaternion that transforms the current vectors into the new ones
    XMVECTOR rotationQuat = XMQuaternionRotationMatrix(XMMatrixLookToLH(newForward, newUp, newRight));

    // Update the rotation quaternion and matrix
    XMStoreFloat4(&qRotation, rotationQuat);
    XMStoreFloat4x4(&mRotation, XMMatrixRotationQuaternion(rotationQuat));
}

void Transform::UpdateRotationFromQuaternion()
{
    // You can implement this function if needed to update the rotation from a quaternion
    // This function is currently empty.
}

void Transform::UpdateRotationFromMatrix()
{
    // Update the rotation quaternion based on the current rotation matrix
    XMStoreFloat4(&qRotation, XMQuaternionRotationMatrix(XMLoadFloat4x4(&mRotation)));

    // Update the direction vectors (vRight, vUp, vForward) based on the rotation matrix
    vRight.x = mRotation._11;
    vRight.y = mRotation._12;
    vRight.z = mRotation._13;
    vUp.x = mRotation._21;
    vUp.y = mRotation._22;
    vUp.z = mRotation._23;
    vForward.x = mRotation._31;
    vForward.y = mRotation._32;
    vForward.z = mRotation._33;
}

void Transform::UpdateMatrix()
{
    // Update the transformation matrix by combining scale, rotation, and translation
    // scaling, rotation, and translation are combined to form the world matrix
    // The direction vectors (vRight, vUp, vForward) are also updated from the rotation matrix
    XMMATRIX scaling = XMLoadFloat4x4(&mScale);
    XMMATRIX rotation = XMLoadFloat4x4(&mRotation);
    XMMATRIX translation = XMLoadFloat4x4(&mPosition);

    XMMATRIX worldMatrix = scaling * rotation * translation;

    // Store the result in mWorldMatrix
    XMStoreFloat4x4(&mWorldMatrix, worldMatrix);

    // Update direction vectors from the rotation matrix
    UpdateRotationFromMatrix();
}

void Transform::UpdateWorldMatrix()
{
    // Update the world matrix by combining the local transformation with the current world matrix
    XMMATRIX localMatrix = XMLoadFloat4x4(&mScale) * XMLoadFloat4x4(&mRotation) * XMLoadFloat4x4(&mPosition);
    XMMATRIX currentWorldMatrix = XMLoadFloat4x4(&mWorldMatrix);

    // Combine the local transformation with the current world matrix
    XMMATRIX newWorldMatrix = localMatrix * currentWorldMatrix;

    // Store the result in the world matrix
    XMStoreFloat4x4(&mWorldMatrix, newWorldMatrix);
}

void Transform::Scale(float x, float y, float z)
{
    // Update the scale vector and matrix
    vScale.x = x;
    vScale.y = y;
    vScale.z = z;

    XMStoreFloat4x4(&mScale, XMMatrixScaling(x, y, z));

    // Update the combined transformation matrix
    UpdateMatrix();
}

void Transform::TranslateInWorld(float x, float y, float z)
{
    // Update the position vector and matrix in world space
    vPosition.x += x;
    vPosition.y += y;
    vPosition.z += z;

    XMStoreFloat4x4(&mPosition, XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z));

    // Update the combined transformation matrix
    UpdateMatrix();
}

void Transform::TranslateInLocal(float x, float y, float z)
{
    // Update the position vector in local space
    vPosition.x += x * vRight.x + y * vUp.x + z * vForward.x;
    vPosition.y += x * vRight.y + y * vUp.y + z * vForward.y;
    vPosition.z += x * vRight.z + y * vUp.z + z * vForward.z;

    // Update the position matrix
    XMStoreFloat4x4(&mPosition, XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z));

    // Update the combined transformation matrix
    UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
    // Set the position to the specified coordinates
    vPosition.x = x;
    vPosition.y = y;
    vPosition.z = z;

    // Update the position matrix
    XMStoreFloat4x4(&mPosition, XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z));

    // Update the combined transformation matrix
    UpdateMatrix();
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
    // Rotate the object by the given yaw, pitch, and roll angles
    // The angles are specified in radians
    XMVECTOR quatRot = XMLoadFloat4(&qRotation);

    yaw = XMConvertToRadians(yaw);
    pitch = XMConvertToRadians(pitch);
    roll = XMConvertToRadians(roll);

    quatRot = XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(pitch, yaw, roll, 1));

    // Update the rotation quaternion and matrix
    XMStoreFloat4(&qRotation, quatRot);
    XMStoreFloat4x4(&mRotation, XMMatrixRotationQuaternion(quatRot));

    // Update the combined transformation matrix
    UpdateMatrix();
}

void Transform::RotateYaw(float angle)
{
    // Rotate the object around the Y-axis (yaw)
    Rotate(angle, 0, 0);
}

void Transform::RotatePitch(float angle)
{
    // Rotate the object around the X-axis (pitch)
    Rotate(0, angle, 0);
}

void Transform::RotateRoll(float angle)
{
    // Rotate the object around the Z-axis (roll)
    Rotate(0, 0, angle);
}
