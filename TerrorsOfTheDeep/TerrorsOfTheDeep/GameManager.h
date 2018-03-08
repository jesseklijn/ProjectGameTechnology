#pragma once
#include <vector>
#include <iostream>
#include "GameObject.h"
using namespace std;


static class GameManager
{
public:
	GameManager();
	~GameManager();
	int GameSpeed = 1;
	bool GamePaused = false;
	
	vector<GameObject> gameObjects;
	void Start();
	void AddGameObject(GameObject gameObject);
	void RemoveGameObject(GameObject gameObject);
	int GetGameObjectIndex(GameObject gameObject);

	enum GameState {
		Main_Menu,
		Level_Selector,
		Credits,
		Settings,
		DemoScene,
	};

	GameState currentGamestate = DemoScene;

	void Update();
	void Draw();

	void GameStateTransition(GameState StateToLoad);
	void UnloadGameState(GameState StateToCleanUp);

private: 
	std::vector<GameObject>::iterator it;
};

