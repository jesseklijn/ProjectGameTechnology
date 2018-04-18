#pragma once
#include "GameManager.h"
#pragma once
#include <utility>
#pragma once
#include "Camera.h"
#pragma once
#include "Monster.h"
#pragma once
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
// Initialize a re-usable ray 
line3d<f32> ray;

// Tracks the current intersection point with the level or a mesh 
vector3df intersection;

// Used to show which triangle has been hit 
triangle3df hitTriangle;
#pragma endregion

#pragma region Variables
std::vector<GameObject*> GameManager::gameObjects;
std::vector<std::string> GameManager::tags;
float GameManager::deltaTime = 0.0;
float GameManager::deltaTimeMS = 0.0;
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
	GameManager::tags.push_back("Ground");
	GameManager::tags.push_back("Key");
	GameManager::tags.push_back("Win");

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

	//std::cout << GameManager::deltaTimeMS << std::endl;
}

// Runs the Draw() for all GameObjects in GameManager::gameObjects.
void GameManager::Draw()
{

}

float GameManager::Min(float value, float value2)
{
	return value <= value2 ? value : value2;
}

float GameManager::Max(float value, float value2)
{
	return value > value2 ? value : value2;
}

float GameManager::Clamp(float value, float minValue, float maxValue)
{
	return GameManager::Max(minValue, GameManager::Min(value, maxValue));
}

float GameManager::Lerp(float value, float value2, float blend)
{
	return value + blend * (value2 - value);
}

irr::core::vector3df GameManager::Lerp(irr::core::vector3df value, irr::core::vector3df value2, float blend)
{
	return vector3df(value.X + blend * (value2.X - value.X),
						value.Y + blend * (value2.Y - value.Y),
						value.Z + blend * (value2.Z - value.Z));
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
			intersection,    // This will be the position of the collision
			hitTriangle,    // This will be the triangle hit in the collision
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
