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
    for (auto it = gameObjects.begin(); it != gameObjects.end();)
    {
        if ((*it)->IsDead())
        {
            delete(*it);
            it = gameObjects.erase(it);
        }
        else
        {
            (*it)->Update(gt);
            ++it;
        }
    }

	//for (GameObject* obj : gameObjects) 
    //{
	//	obj->Update(gt);
	//}

	// Collision
    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t k = i + 1; k < gameObjects.size(); ++k) {
            if (gameObjects[i]->HasCollider() && gameObjects[k]->HasCollider() &&
                gameObjects[i]->GetCollider()->IsColliding(*gameObjects[k])) {
                delete gameObjects[i];
                delete gameObjects[k];
                gameObjects.erase(gameObjects.begin() + k);
                gameObjects.erase(gameObjects.begin() + i);
                i = 0; // Restart the iteration after removal
                break;  // Exit the inner loop after a collision is found
            }
        }
    }
}

vector<GameObject*>& Manager::GetGameObjects()
{
	return gameObjects;
}
