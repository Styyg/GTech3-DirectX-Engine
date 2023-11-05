#include "GameObject.h"
//#include "Collider.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::AddCollider(Collider collider)
{
	mCmps.emplace(Component::COLLIDER, collider);

}

void GameObject::AddRigidbody(Rigidbody rigidbody)
{
	mCmps.emplace(Component::RIGIDBODY, rigidbody);
}

Collider* GameObject::GetCollider() {
    // Utilise la fonction find pour chercher le composant Collider dans mCmps.
    auto it = mCmps.find(Component::COLLIDER);

    // Vérifie si le composant Collider est trouvé.
    if (it != mCmps.end()) {
        // Si le composant Collider est trouvé, renvoie un pointeur vers celui-ci.
        return dynamic_cast<Collider*>(it->second);
    }
    else {
        // Le composant Collider n'est pas trouvé, renvoie nullptr.
        return nullptr;
    }
}


//void GameObject::AddMesh(Mesh mesh)
//{
//	mCmps.emplace(Component::MESH, mesh);
//}
/*
void GameObject::AddTrigger(Trigger trigger)
{
	mCmps.emplace(TRIGGER, trigger);

}*/

void GameObject::Render()
{

}

void GameObject::update()
{

}