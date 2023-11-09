#include "Missile.h"

Missile::Missile(Camera* cam) :
    //position(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    //direction(XMFLOAT3(0.0f, 0.0f, 1.0f)),
    velocity(500.0f)
{
    AddCollider();
    XMFLOAT3 rota;
    XMFLOAT3 lookAtf = cam->GetLookAt();
    XMFLOAT3 posf = cam->GetPosition();
    XMVECTOR lookAt = XMLoadFloat3(&lookAtf);
    XMVECTOR pos = XMLoadFloat3(&posf);
    XMStoreFloat3(&rota, XMVectorSubtract(lookAt, pos));
    float factor = 620000;
    mTransform.Rotate(rota.x * factor, -rota.y * factor, rota.z * factor);
}

Missile::~Missile() {}

void Missile::Update(GameTimer gt) {
    // Mettre à jour la position en fonction de la direction et de la vitesse.
    //position.x += direction.x * velocity * gt.DeltaTime();
    //position.y += direction.y * velocity * gt.DeltaTime();
    //position.z += direction.z * velocity * gt.DeltaTime();

    //mTransform.Rotate(0, 0, 0);

    if (mBirthTime + mLifeSpan < gt.TotalTime())
        Kill();
    else
        mTransform.TranslateInLocal(0, 0, velocity * gt.DeltaTime());
}

void Missile::SetBirthTime(float birthTime)
{
    mBirthTime = birthTime;
}

//XMFLOAT3 Missile::GetPosition() const { return position; }
//void Missile::SetPosition(const XMFLOAT3& newPos) { position = newPos; }
//
//XMFLOAT3 Missile::GetDirection() const { return direction; }
//void Missile::SetDirection(const XMFLOAT3& newDir) { direction = newDir; }
//
//float Missile::GetVelocity() const { return velocity; }
//void Missile::SetVelocity(float newVelocity) { velocity = newVelocity; }
