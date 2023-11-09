#pragma once

#include <iostream>
#include <vector>

#include "GameObject.h"

using namespace std;

class Manager {
public:
    static Manager* GetInstance();

    void AddGameObject(GameObject* obj);
    //void RemoveGameObject(const GameObject& obj);
    void ClearGameObjects();
    void Update();

    vector<GameObject*>& GetGameObjects();

private:
    vector<GameObject*> gameObjects;

    Manager() {}
};