#pragma once

#include <iostream>
#include <list>

#include "GameObject.h"

using namespace std;

class Manager {
public:
    static Manager* GetInstance();

    void AddGameObject(const GameObject& obj);
    //void RemoveGameObject(const GameObject& obj);
    void ClearGameObjects();

    list<GameObject>& GetGameObjects();

private:
    list<GameObject> gameObjects;

    Manager() {}
};