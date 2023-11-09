#include "Missile.h"

Missile::Missile() :
    position(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    direction(XMFLOAT3(1.0f, 0.0f, 0.0f)),
    velocity(0.0f)
{}

Missile::~Missile() {}

void Missile::Update(float deltaTime) {
    // Mettre à jour la position en fonction de la direction et de la vitesse.
    position.x += direction.x * velocity * deltaTime;
    position.y += direction.y * velocity * deltaTime;
    position.z += direction.z * velocity * deltaTime;
}

XMFLOAT3 Missile::GetPosition() const { return position; }
void Missile::SetPosition(const XMFLOAT3& newPos) { position = newPos; }

XMFLOAT3 Missile::GetDirection() const { return direction; }
void Missile::SetDirection(const XMFLOAT3& newDir) { direction = newDir; }

float Missile::GetVelocity() const { return velocity; }
void Missile::SetVelocity(float newVelocity) { velocity = newVelocity; }