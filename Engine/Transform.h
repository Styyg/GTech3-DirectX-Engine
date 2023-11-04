#pragma once
#include <DirectXMath.h>

class Transform
{
public:
    DirectX::XMFLOAT3 vPosition; // Translation vector for the object
    DirectX::XMFLOAT4X4 mPosition; // Transformation matrix for translation
    
    DirectX::XMFLOAT3 vScale; // Scale factors along the x, y, and z axes
    DirectX::XMFLOAT4X4 mScale; // Transformation matrix for scaling
    
    DirectX::XMFLOAT4 qRotation; // Quaternion representing rotation
    DirectX::XMFLOAT4X4 mRotation; // Transformation matrix for rotation
    
    DirectX::XMFLOAT3 vForward; // Forward vector in local space
    DirectX::XMFLOAT3 vRight; // Right vector in local space
    DirectX::XMFLOAT3 vUp; // Up vector in local space
    
    DirectX::XMFLOAT4X4 mWorldMatrix; // Combined transformation matrix for the world
    
    Transform(); // Constructor for the Transform class
   

    void InitializeIdentity();  // Initialize transformation matrices as identity matrices
    void FromMatrix(DirectX::XMMATRIX* pMat);  // Decompose and update transformation from a given matrix

    void UpdateRotationFromVectors();  // Update rotation from the forward, right, and up vectors
    void UpdateRotationFromQuaternion();  // Not implemented in this code
    void UpdateRotationFromMatrix();  // Update rotation from the rotation matrix

    void UpdateMatrix();  // Update the combined transformation matrix
    void UpdateWorldMatrix();  // Update the world matrix by combining local transformation with the current world matrix

    void Scale(float x, float y, float z);  // Update the scale of the object
    void TranslateInWorld(float x, float y, float z);  // Translate the object in world space
    void TranslateInLocal(float x, float y, float z);  // Translate the object in local space
    void SetPosition(float x, float y, float z);  // Set the position of the object

    void Rotate(float yaw, float pitch, float roll);  // Rotate the object in yaw, pitch, and roll angles
    void RotateYaw(float angle);  // Rotate the object around the Y-axis
    void RotatePitch(float angle); // Rotate the object around the X-axis
    void RotateRoll(float angle);  // Rotate the object around the Z-axis
};


