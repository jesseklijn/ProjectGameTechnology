#pragma once
#include <vector>
#include <iostream>


using namespace std;


static class GameManager
{
public:
	GameManager();
	~GameManager();
	int GameSpeed = 1;
	bool GamePaused = false;

	enum GameState {
		Main_Menu,
		Level_Selector,
		Credits,
		Settings,
		DemoScene,
	};

	GameState currentGamestate = DemoScene;

	void Awake();
	void Start();
	void Update();
	void Draw();

	void GameStateTransition(GameState StateToLoad);
	void UnloadGameState(GameState StateToCleanUp);

private: 
};

