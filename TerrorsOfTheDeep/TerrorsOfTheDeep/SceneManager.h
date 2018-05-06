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
	static SceneType scene, scenePrevious;
	static bool sceneIsPaused;

	// Light data
	static irr::video::SColorf ambientColor, flashlightColor, sharkEyesColor;
	static vector3df chestLightOffset, keyLightOffset;

	static Camera* camera;
	static HUD* hud;

	// Whether to hide or show the HUD
	static bool disableHud;


	// Functions
	virtual void Update();
	virtual void Draw();
	virtual void DrawGUI() {};
	virtual Tag GetTag();

	static bool LoadScene(SceneType sceneToLoad);
	static void PauseScene(bool mode);
	static void StartLoadingScreen();
	static void EndLoadingScreen();
	static void OnSceneChange();
};

