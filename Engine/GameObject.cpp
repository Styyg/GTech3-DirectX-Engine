#include "GameObject.h"
//#include "Collider.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::AddCollider(Collider collider)
{
	mCmps.emplace(Component::COLLIDER, collider);

}

void GameObject::AddRigidbody(Rigidbody rigidbody)
{
	mCmps.emplace(Component::RIGIDBODY, rigidbody);
}

//void GameObject::AddMesh(Mesh mesh)
//{
//	mCmps.emplace(Component::MESH, mesh);
//}
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