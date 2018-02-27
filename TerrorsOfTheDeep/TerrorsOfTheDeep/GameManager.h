#pragma once
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

	static void Update();
	void GameStateTransition(GameState StateToLoad);
	void UnloadGameState(GameState StateToCleanUp);

};

