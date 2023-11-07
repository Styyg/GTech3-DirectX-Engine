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

void Manager::Update()
{
	float i = 0.1f;
	for (GameObject* obj : gameObjects) {

		// Update Object CBs
		ObjectConstants objConstants;
		obj->mTransform.SetPosition(i, 0, 0);
		i += .5f;
		XMMATRIX world = XMLoadFloat4x4(&obj->mTransform.mWorldMatrix);
		XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(world));
		//objConstants.WorldViewProj = mWorldViewProj;
		obj->mObjectCB->CopyData(0, objConstants);

		//obj->Update();

		// Update Object pass constants
		//PassConstants passConstants;
		//XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, 800.f/600, 1.0f, 1000.0f);

		//XMMATRIX viewProj = XMMatrixMultiply(view, proj);
		//XMStoreFloat4x4(&passConstants.ViewProj, XMMatrixTranspose(viewProj));

		//obj->mPassConstants->CopyData(0, passConstants);
	}
}

list<GameObject*>& Manager::GetGameObjects()
{
	return gameObjects;
}
