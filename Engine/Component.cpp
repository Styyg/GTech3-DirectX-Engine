#include "Component.h"

Component::Component(GameObject* gameObject)
{
	mGameObjectPtr = gameObject;
}

Component::~Component()
{
}
