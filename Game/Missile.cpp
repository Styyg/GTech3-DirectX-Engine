#include "Missile.h"

Missile::Missile(Camera* cam) :
    position(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    direction(XMFLOAT3(0.0f, 0.0f, 1.0f)),
    velocity(1.0f)
{
    mTransform.Rotate(0, 0, 0);
}

Missile::~Missile() {}

void Missile::Update(GameTimer gt) {
    // Mettre à jour la position en fonction de la direction et de la vitesse.
    position.x += direction.x * velocity * gt.DeltaTime();
    position.y += direction.y * velocity * gt.DeltaTime();
    position.z += direction.z * velocity * gt.DeltaTime();

    mTransform.Rotate(0, 0, 0);
    mTransform.TranslateInLocal(0, 0, velocity * gt.DeltaTime());
}

//XMFLOAT3 Missile::GetPosition() const { return position; }
//void Missile::SetPosition(const XMFLOAT3& newPos) { position = newPos; }
//
//XMFLOAT3 Missile::GetDirection() const { return direction; }
//void Missile::SetDirection(const XMFLOAT3& newDir) { direction = newDir; }
//
//float Missile::GetVelocity() const { return velocity; }
//void Missile::SetVelocity(float newVelocity) { velocity = newVelocity; }