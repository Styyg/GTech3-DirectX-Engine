#pragma once
#include <iostream>
#include <vector>
#include "Component.h"
#include "Transform.h"

using namespace std;

class GameObject
{
private:
	string mName;
	Transform mTransform;
	vector<Component>mCmps;
public:
	GameObject();
	~GameObject();
	void AddComponent(Component component);
};

