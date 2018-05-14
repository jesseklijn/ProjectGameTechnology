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

// Forward declaration
class Menu;

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

	// Components for every scene
	static Camera* camera;
	static HUD* hud;
	static GameObject* levelMonster;
	static GameObject* levelPlayer;
	static GameObject* divingCage;
	static bool disableHud;
	static irr::core::vector3df rayStart;
	static irr::core::vector3df rayEnd;

	// Intro
	static bool introIsActive, cageShouldDescend, showControls, showMouseOverlay, showKeyOverlay;
	static float introStartTime, introStartTimer;
	static float introMouseOverlayTime, introMouseOverlayTimer;
	static float introMouseOverlayDisplayTime, introMouseOverlayDisplayTimer;
	static float introKeyOverlayTime, introKeyOverlayTimer;
	static float introKeyOverlayDisplayTime, introKeyOverlayDisplayTimer;
	static float introCageDescendRate;

	static Menu* mouseOverlay;
	static Menu* keyOverlay;	


	// Functions
	virtual void Update();
	virtual void Draw();
	virtual void DrawGUI() {};
	virtual Tag GetTag();

	static bool LoadScene(SceneType sceneToLoad);
	static void PauseScene(bool mode);
	static void OnSceneChange();

	static void StartLoadingScreen();
	static void EndLoadingScreen();

	static void StartLevelIntro();
	static void EndLevelIntro();
	static void ShowMouseControlsOverlay();
	static void ShowKeyControlsOverlay();
	static void HideMouseControlsOverlay();
	static void HideKeyControlsOverlay();
};