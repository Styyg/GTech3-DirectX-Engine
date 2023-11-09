#pragma once

#include "GameObject.h"

struct Vector3D {
	float x, y, z;

	Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

class Missile : public GameObject
{
public:
	Missile(Vector3D pos, Vector3D dir, float speed);
	~Missile();

	void Update(float deltaTime);

	Vector3D GetPosition() const;
	void SetPosition(const Vector3D& newPos);

	Vector3D GetDirection() const;
	void SetDirection(const Vector3D& newDir);

	float GetVelocity() const;
	void SetVelocity(float newVelocity);

private:
	Vector3D position;
	Vector3D direction;
	float velocity;
};