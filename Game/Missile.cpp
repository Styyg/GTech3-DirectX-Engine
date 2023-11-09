#include "Missile.h"

Missile::Missile(Vector3D pos, Vector3D dir, float speed) : position(pos), direction(dir), velocity(speed) {}

Missile::~Missile() {}

void Missile::Update(float deltaTime) {
    // Mettre à jour la position en fonction de la direction et de la vitesse.
    position.x += direction.x * velocity * deltaTime;
    position.y += direction.y * velocity * deltaTime;
    position.z += direction.z * velocity * deltaTime;
}

// Accesseurs et mutateurs pour la position, la direction et la vélocité.
Vector3D Missile::GetPosition() const { return position; }
void Missile::SetPosition(const Vector3D& newPos) { position = newPos; }

Vector3D Missile::GetDirection() const { return direction; }
void Missile::SetDirection(const Vector3D& newDir) { direction = newDir; }

float Missile::GetVelocity() const { return velocity; }
void Missile::SetVelocity(float newVelocity) { velocity = newVelocity; }