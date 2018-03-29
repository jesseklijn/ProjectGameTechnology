#pragma once
#include "GameManager.h"
#pragma once
#include <utility>
#pragma once
#include "Camera.h"
#pragma once
#include "Monster.h"
#include "GridMesh.h"
#include "EventManager.h"

// If this runs on Windows, link with the Irrlicht lib file. Also disable the default C++ console window
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


#pragma region Core Irrlicht Components
// Initialize Irrlicht device

EventManager GameManager::eventManager;

irr::IrrlichtDevice* GameManager::device =
createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1024, 720), 64,
	false, true, false, &eventManager);

// Initialize Irrlicht components
irr::video::IVideoDriver* GameManager::driver = GameManager::device->getVideoDriver();
irr::scene::ISceneManager* GameManager::smgr = GameManager::device->getSceneManager();
irr::gui::IGUIEnvironment* GameManager::guienv = GameManager::device->getGUIEnvironment();
irr::scene::ISceneCollisionManager* GameManager::collMan = GameManager::smgr->getSceneCollisionManager();
irr::gui::IGUIFont* GameManager::font = GameManager::guienv->getBuiltInFont();
#pragma endregion

#pragma region Raycasting
// Initialize a base selector, used for assigning selection to scene nodes 
// It's dropped after every selector assignment, but it's easily re-usable 
ITriangleSelector* selector;

// Initialize a re-usable ray 
line3d<f32>* ray;

// Tracks the current intersection point with the level or a mesh 
vector3df* intersection;

// Used to show which triangle has been hit 
triangle3df* hitTriangle;
#pragma endregion

#pragma region Variables
std::vector<GameObject*> GameManager::gameObjects;
std::vector<std::string> GameManager::tags;
#pragma endregion

// Constructor
GameManager::GameManager()
{
	// Set a default font
	GameManager::guienv->getSkin()->setFont(GameManager::device->getGUIEnvironment()->getBuiltInFont());

	// Set up tags
	GameManager::tags.push_back("<NONE>");
	GameManager::tags.push_back("Player");
	GameManager::tags.push_back("Monster");
	GameManager::tags.push_back("World Object");

	GridMesh playingMesh = GridMesh(
		new const vector3df(0, 0, 0),
		new const vector3df(1, 1, 1),
		new const vector3df(0, 0, 0),
		0,
		GameManager::smgr,
		0);
	Awake();
}

// Destructor
GameManager::~GameManager()
{

}

// Runs immediately after the constructor once
void GameManager::Awake()
{
	Start();
}

// Runs similarly to Awake(), but later
void GameManager::Start()
{

}

// Runs the Update() for all GameObjects in GameManager::gameObjects.
void GameManager::Update()
{
	for (int i = 0; i < GameManager::gameObjects.size(); ++i)
	{
		GameManager::gameObjects[i]->Update();
	}
}

// Runs the Draw() for all GameObjects in GameManager::gameObjects.
void GameManager::Draw()
{

}

// Switch to the given GameState.
// TODO: Functionality!
void GameManager::GameStateTransition(GameState StateToLoad)
{

}

// Cleans up the given state.
// TODO: Functionality!
void GameManager::UnloadGameState(GameState StateToCleanUp)
{

}

/* Cast a Raycast line between the given start and end positions and return the ISceneNode that was hit.
NOTE: In order for raycast detection, nodes must be flagged as such first by linking a selector to it.

Example:
	selector = GameManager::smgr->createTriangleSelector(<NODE>);
	<NODE>->setTriangleSelector(selector);
	selector->drop();
*/
ISceneNode* GameManager::PerformRaycast(vector3df startPosition, vector3df endPosition)
{
	line3d<f32> ray;
	ray.start = startPosition;
	ray.end = endPosition;

	// This call is all you need to perform ray/triangle collision on every scene node
	// that has a triangle selector, including the Quake level mesh.  It finds the nearest
	// collision point/triangle, and returns the scene node containing that point.
	// Irrlicht provides other types of selection, including ray/triangle selector,
	// ray/box and ellipse/triangle selector, plus associated helpers.
	// See the methods of ISceneCollisionManager
	ISceneNode* selectedSceneNode =
		GameManager::collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			*intersection,    // This will be the position of the collision
			*hitTriangle,    // This will be the triangle hit in the collision
			GameManager::IDFlag_IsPickable,  // This ensures that only nodes that we have set up to be pickable are considered
			0);          // Check the entire scene (this is actually the implicit default)

	return selectedSceneNode;
}

// Finds the first GameObject that matches the given tag.
GameObject* GameManager::FindGameObjectWithTag(std::string name)
{
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj->GetTag() == name)
			return gameObj;
	return nullptr;
}
