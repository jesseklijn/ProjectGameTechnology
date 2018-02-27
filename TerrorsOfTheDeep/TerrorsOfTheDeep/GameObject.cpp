#include "GameObject.h"
#include "GameManager.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
	GameManager manager;
	manager.Update();
}
