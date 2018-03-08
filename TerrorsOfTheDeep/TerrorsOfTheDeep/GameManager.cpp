#include "GameManager.h"
#include <utility>


GameManager::GameManager()
{
	GameObject test;
	gameObjects.push_back(test);
}


GameManager::~GameManager()
{

}

void GameManager::Start()
{
}

/* Adds GameObject to collection of Game Manager*/
void GameManager::AddGameObject(GameObject gameObject)
{
	gameObjects.push_back(gameObject);
}
/* Removes the given gameObject from the gamemanagers collection */
void GameManager::RemoveGameObject(GameObject gameObject)
{
	if (gameObjects.size() > 0) {
		gameObjects.erase(gameObjects.begin() + GetGameObjectIndex(gameObject));
	}
}
/* Returns the index of the gameObject in the GameManagers collection of gameobjects */
int GameManager::GetGameObjectIndex(GameObject gameObject)
{
	int idx = distance(gameObjects.begin(), it);
	return idx;



}

void GameManager::Update()
{
	if (gameObjects.size() > 0) {
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i].Update();
		}
	}
}

void GameManager::Draw()
{
	if (gameObjects.size() > 0) {
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i].Draw();
		}
	}
}


void GameManager::GameStateTransition(GameState StateToLoad)
{

}

void GameManager::UnloadGameState(GameState StateToCleanUp)
{

}
