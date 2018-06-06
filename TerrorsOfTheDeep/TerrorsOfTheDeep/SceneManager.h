#pragma region Includes
#pragma once
#include <irrlicht.h>
#include "Lighting.h"
#include "HUD.h"
#include "Player.h"
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
class Fader;

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
		LOADING,
		GAME_OVER
	};

	enum LoadingType
	{
		NO_OBJECTS,
		BASICS,
		PLAYER,
		CREATURES,
		CRITTERS,
		RELICS,
		CORALS,
		VINES,
		RUINS,
		TERRORS,
		SHIPS,
		WORLD,
		LAND_MARKS
	};

	enum FaderAction
	{
		NO_ACTION,
		SCENE_SWITCH_TO_LEVEL,
		SCENE_SWITCH_TO_MAIN
	};
	static FaderAction faderAction;

	static SceneType scene, scenePrevious;
	static bool sceneIsPaused, sceneIsLoaded;

	// Light data
	static irr::video::SColorf ambientColor, flashlightColor, sharkEyesColor;
	static vector3df chestLightOffset, keyLightOffset;

	// Components for every scene
	static Camera* camera;
	static HUD* hud;
	static GameObject* levelMonster;
	static GameObject* levelPlayer;
	static IAnimatedMeshSceneNode* levelPlane;
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
	static Menu* pauseMenu;
	static Fader* fader;

	// Default parameters
	static vector3df* vectorZero; // Short for new vector3df(0,0,0)
	static int noID; // ID that isn't going to be used
	static GameObject* defaultGameObject; // Empty GameObject
	static std::vector<GameObject*> defaultGameObjectList; // Empty GameObjectList
	static float distanceKeyChest;

	// Functions
	virtual void Update();
	virtual void Draw();
	virtual void DrawGUI() {};
	virtual Tag GetTag();

	static bool LoadScene(SceneType sceneToLoad);
	static void PauseScene(bool mode);
	static void OnSceneChange();

	static void StartLoadingScreen(LoadingType loadingType);
	static void EndLoadingScreen();

	static void StartLevelIntro();
	static void EndLevelIntro();
	static void ShowMouseControlsOverlay();
	static void ShowKeyControlsOverlay();
	static void HideMouseControlsOverlay();
	static void HideKeyControlsOverlay();

	static void TriggerFaderAction();
};
