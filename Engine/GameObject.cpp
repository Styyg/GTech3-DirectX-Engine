#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component component)
{
	mCmps.push_back(component);

}