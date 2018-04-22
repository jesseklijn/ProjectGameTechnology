#pragma region Includes
#pragma once
#include <vector>
#pragma once
#include "GameObject.h"
#pragma once
#include "EventManager.h"
#pragma once
#include <iostream>
#pragma once
#include "irrlicht.h"
#pragma endregion

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion


// Class responsible for managing the overall game.
// Makes use of state tracking and switching.
// Runs Update() and Draw() calls for every GameObject.
class GameManager
{
public:

#pragma region Core Irrlicht Components
	// Define Irrlicht device
	static irr::IrrlichtDevice* device;

	// Define core Irrlicht components
	static irr::video::IVideoDriver* driver;
	static irr::scene::ISceneManager* smgr;
	static irr::gui::IGUIEnvironment* guienv;
	static irr::scene::ISceneCollisionManager* collMan;
	static irr::gui::IGUIFont* font;
#pragma endregion

#pragma region Raycasting
	// Define enums for raycast selections. 
	// These flags will determine how something is selected 
	enum
	{
		/* Use this ISceneNode ID to indicate a scene node that is 
		not pickable by getSceneNodeAndCollisionPointFromRay()*/ 
		ID_IsNotPickable = 0,

		/* Use this flag in ISceneNode IDs to indicate that the 
		scene node can be picked by ray selection.*/ 
		IDFlag_IsPickable = 1 << 0,

		/* Use this flag in ISceneNode IDs to indicate that the 
		scene node can be highlighted.*/ 
		IDFlag_IsHighlightable = 1 << 1
	};
#pragma endregion

#pragma region Game state
	enum GameState {
		MAINMENU,
		LEVELSELECTOR,
		CREDITS,
		SETTINGS,
		DEMOSCENE,
	};
	GameState currentGamestate = DEMOSCENE;
#pragma endregion

	// Constructor and destructor
	GameManager();
	~GameManager();

	// Variables

	// Event receiver
	static EventManager eventManager;

	// A list of all GameObjects or children of GameObjects in the scene
	static std::vector<GameObject*> gameObjects;

	/* A list of all grid cell lists, containing GameObjects.
	
	Getting the right grid cell list requires a conversion from 3D to 1D coordinates as followed:
	partGrid[x * GameManager::partGridSizeY * GameManager::partGridSizeZ + y * GameManager::partGridSizeZ + z]
	*/
	static std::vector<vector<GameObject*>> partGrid;

	// Delta timing / Time
	static float deltaTime, deltaTimeMS, deltaTimeFixed, deltaTimeFixedMS, fixedTimeStep, time;

	static const int worldRadiusX, worldRadiusY, worldRadiusZ;
	static const int partGridCellSizeX, partGridCellSizeY, partGridCellSizeZ, partGridSizeX, partGridSizeY, partGridSizeZ;
	static float creatureStateRange;

	int GameSpeed = 1;
	bool GamePaused = false;		

	// Functions
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Draw();

	void GameStateTransition(GameState StateToLoad);
	void UnloadGameState(GameState StateToCleanUp);

	static scene::ISceneNode* PerformRaycast(core::vector3df startPosition, core::vector3df endPosition);

	static int FindTagInTagList(std::vector<GameObject::Tag> vectorList, GameObject::Tag listTag);
	static GameObject* FindGameObjectWithTag(GameObject::Tag tag);
	static std::vector<GameObject*> FindGameObjectsWithTag(GameObject::Tag tag); 
	static std::vector<GameObject*> FindGameObjectsWithTags(std::vector<GameObject::Tag> tagList);
	static GameObject* FindNearestGameObjectWithTag(GameObject* origin, GameObject::Tag name, float detectionRange = INFINITY, bool visibilityCheck = false); 
	static GameObject* FindNearestGameObjectWithTags(GameObject* origin, std::vector<GameObject::Tag> tagList, float detectionRange = INFINITY, bool visibilityCheck = false);
	static GameObject* FindFurthestGameObjectWithTag(GameObject* origin, GameObject::Tag name, float detectionRange = INFINITY, bool visibilityCheck = false);
	static GameObject* FindFurthestGameObjectWithTags(GameObject* origin, std::vector<GameObject::Tag> tagList, float detectionRange = INFINITY, bool visibilityCheck = false);

	static float Min(float value, float value2);
	static float Max(float value, float value2);
	static float Clamp(float value, float minValue, float maxValue);
	static float Lerp(float value, float value2, float blend);
	static irr::core::vector3df Lerp(irr::core::vector3df value, irr::core::vector3df value2, double blend);

private:
	float fixedTime = 0.0f, fixedCorrection;
};