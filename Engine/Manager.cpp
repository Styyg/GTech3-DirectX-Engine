#include "Manager.h"

Manager* Manager::GetInstance()
{
	static Manager manager;
	return &manager;
}

void Manager::AddGameObject(GameObject* obj)
{
	gameObjects.push_back(obj);
}

//void Manager::RemoveGameObject(const GameObject& obj) {
//	gameObjects.remove(obj);
//}

void Manager::ClearGameObjects()
{
	gameObjects.clear();
}

void Manager::Update(GameTimer gt)
{
	for (GameObject* obj : gameObjects)
	{
		obj->Update(gt);
	}
}

list<GameObject*>& Manager::GetGameObjects()
{
	return gameObjects;
}
