#pragma once

#include <iostream>
#include <vector>

#include "Component.h"
#include "Transform.h"

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component component);

private:
	string mName;
	Transform mTransform;
	vector<Component>mCmps;
};

