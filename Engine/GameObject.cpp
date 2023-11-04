#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::AddCollider(Collider collider)
{
	mCmps.emplace(COLLIDER, collider);

}

void GameObject::AddRigidbody(Rigidbody rigidbody)
{
	mCmps.emplace(RIGIDBODY, rigidbody);

}

void GameObject::AddMesh(Mesh mesh)
{
	mCmps.emplace(MESH, mesh);

}
/*
void GameObject::AddTrigger(Trigger trigger)
{
	mCmps.emplace(TRIGGER, trigger);

}*/

void GameObject::Render()
{

}

void GameObject::update()
{

}