#pragma region Includes
#pragma once
#include <irrlicht.h>
#include "Lighting.h"
#include "HUD.h"
#include "Player.h"
#include "GridMesh.h"
#include "Camera.h"
#include "DetectCollision.h"
#include "Sound.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Menu.h"
#include "Critter.h"
#include "Shark.h"
#include <string>
#include <iostream>
#include "FlockingEntity.h"
#include <chrono>
#pragma endregion

/* Class responsible for managing different scenes in the game.
This includes scenes like a title screen, loading screen etcetera. */
class SceneManager : public DynamicUpdater
{
public:
	SceneManager();
	~SceneManager();


	enum SceneType
	{
		NONE,
		TITLE_SCREEN,
		LEVEL,
		GAME_OVER
	};
	SceneType scene = NONE;
	SceneType scenePrevious = scene;
	bool sceneIsPaused = false;

	const int NO_PARENT = 0;
	const float KEYLIGHT_RADIUS = 50.f;
	const float CHESTLIGHT_RADIUS = 90.f;
	const float FLASHLIGHT_RANGE = 1500.f;

	// Light data
	irr::video::SColorf ambientColor = irr::video::SColor(255, 30, 30, 50);
	//irr::video::SColorf ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
	irr::video::SColorf flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
	irr::video::SColorf sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);	
	vector3df chestLightOffset = vector3df(40, 30, 0);
	vector3df keyLightOffset = vector3df(0, 20, 0);

	Camera* camera;
	HUD* hud = new HUD;

	// Whether to hide or show the HUD
	bool disableHud = false;


	// Functions
	virtual void Update();
	virtual void Draw();
	virtual void DrawGUI() {};
	virtual Tag GetTag();

	bool LoadScene(SceneType sceneToLoad);
	void PauseScene(bool mode);
	void StartLoadingScreen();
	void EndLoadingScreen();
	void OnSceneChange();
};

