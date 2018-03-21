#pragma region Includes
#pragma once
#include <vector>
#pragma once
#include "GameObject.h"
#pragma once
#include <iostream>
#pragma once
#include "irrlicht.h"
#pragma once
#include <vector>
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
	//Create Irrlicht device
	static irr::IrrlichtDevice* device;

	// Get core Irrlicht components
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
		// I use this ISceneNode ID to indicate a scene node that is 
		// not pickable by getSceneNodeAndCollisionPointFromRay() 
		ID_IsNotPickable = 0,

		// I use this flag in ISceneNode IDs to indicate that the 
		// scene node can be picked by ray selection. 
		IDFlag_IsPickable = 1 << 0,

		// I use this flag in ISceneNode IDs to indicate that the 
		// scene node can be highlighted.  In this example, the 
		// homonids can be highlighted, but the level mesh can't. 
		IDFlag_IsHighlightable = 1 << 1
	};
#pragma endregion

#pragma region Game state
	enum GameState {
		Main_Menu,
		Level_Selector,
		Credits,
		Settings,
		DemoScene,
	};
	GameState currentGamestate = DemoScene;
#pragma endregion

	// Constructor and destructor
	GameManager();
	~GameManager();

	// Variables

	// A list of all GameObjects in the scene
	// Once created, a GameObject automatically adds itself to this list in its constructor
	//static std::vector<GameObject>* gameObjects;

	int GameSpeed = 1;
	bool GamePaused = false;

	// Functions
	void Awake();
	void Start();
	void Update();
	void Draw();

	void GameStateTransition(GameState StateToLoad);
	void UnloadGameState(GameState StateToCleanUp);

	static scene::ISceneNode* PerformRaycast(core::vector3df startPosition, core::vector3df endPosition);

private:
	typedef std::vector<GameObject*> VITEM;
	VITEM m_vec;
};