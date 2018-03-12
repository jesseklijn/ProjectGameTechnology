#pragma once
#include <vector>
#include <iostream>
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std; 

// Define enums for raycast selections. 
// These flags will determine how something is selected 
static enum
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

#pragma region Core Irrlicht Components
//Create Irrlicht device
static irr::IrrlichtDevice *device =
createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
	false, false, false, 0);

// Get core Irrlicht components
static irr::video::IVideoDriver *driver = device->getVideoDriver();
static irr::scene::ISceneManager *smgr = device->getSceneManager();
static irr::gui::IGUIEnvironment *guienv = device->getGUIEnvironment();
static irr::scene::ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();
static irr::gui::IGUIFont *font = guienv->getBuiltInFont();
#pragma endregion

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

