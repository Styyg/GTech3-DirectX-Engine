#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* gameObject)
{
	mGameObjectPtr = gameObject;
}

Component::~Component()
{
}
