#pragma once
#include "GameManager.h"
#include <utility>
#include <algorithm>
#include "Camera.h"
#include "Monster.h"
#include "GridMesh.h"
#include "EventManager.h"

// If this runs on Windows, link with the Irrlicht lib file. Also disable the default C++ console window
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


#pragma region Core Irrlicht Components
// Initialize Irrlicht device

EventManager GameManager::eventManager;

irr::IrrlichtDevice* GameManager::device =
	createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1920, 1080), 64,
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
std::vector<InterfaceObject*> GameManager::interfaceObjects;

GameObject* GameManager::levelMonster = nullptr;
GameObject* GameManager::levelPlayer = nullptr;

// Timing
float GameManager::deltaTime = 0.0;
float GameManager::deltaTimeMS = 0.0;
float GameManager::deltaTimeFixed = 0.0;
float GameManager::deltaTimeFixedMS = 0.0f;
float GameManager::time = 0.0;
float GameManager::fixedTimeStep = 60.0f;

float GameManager::creatureStateRange = 25000.0f;

bool GameManager::gameOver = false;

const irr::core::dimension2du& GameManager::screenDimensions = GameManager::driver->getScreenSize();

// World dimensions
const int GameManager::WORLD_RADIUS_X = 12000.0f;
const int GameManager::WORLD_RADIUS_Y = 9000.0f;
const int GameManager::WORLD_RADIUS_Z = 12000.0f;
bool GameManager::keyPickedUp = false;
bool GameManager::escaped = false;
bool GameManager::hasDied = false;

// World generation
int GameManager::critterCount = 200;
int GameManager::shipCount = 6;
int GameManager::rockCount = 50;
int GameManager::ruinsCount = 24;
int GameManager::coralCount = 135;
int GameManager::plantCount = 40;
int GameManager::skullCount = 2;

float GameManager::gameSpeed = 1.0f;
#pragma endregion

// Constructor
GameManager::GameManager()
{
	// NOTE: if EFT_FOG_EXP / EFT_FOG_EXP2, distances don't matter, only density!
	GameManager::driver->setFog(SColor(1, 0, 0, 25), EFT_FOG_EXP2, 0.0f, 5000.0f, 0.0002f);
	GameManager::guienv->getSkin()->setFont(GameManager::device->getGUIEnvironment()->getBuiltInFont());
	
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

void GameManager::Update()
{
	fixedTime += GameManager::deltaTime;
	eventManager.Update();

	/* Runs the Update() for all objects in GameManager.
	Used for basic updates per frame. */
	for (int gIndex = 0; gIndex < GameManager::gameObjects.size(); gIndex++)
		GameManager::gameObjects[gIndex]->Update();
	for (int iIndex = 0; iIndex < GameManager::interfaceObjects.size(); iIndex++)
		GameManager::interfaceObjects[iIndex]->Update();
		
	/* Runs the FixedUpdate() for all objects in GameManager.
	Used for fixed updates at specific timestep intervals, ideally for physics updates. */
	if (fixedTime >= 1.0f / GameManager::fixedTimeStep)
	{
		fixedCorrection = (fixedTime - 1.0f / GameManager::fixedTimeStep);
		GameManager::FixedUpdate();
	}
}

/* Runs similar to Update();, but after a predetermined timestep. */
void GameManager::FixedUpdate()
{
	fixedTime = 0.0f;
	GameManager::deltaTimeFixed = (1.0f / GameManager::fixedTimeStep + fixedCorrection) * GameManager::gameSpeed;
	GameManager::deltaTimeFixedMS = GameManager::deltaTimeFixed * 1000.0f * GameManager::gameSpeed;

	// Runs the FixedUpdate() for all GameObjects in GameManager::gameObjects.
	for (int i = 0; i < GameManager::gameObjects.size(); i++)
		GameManager::gameObjects[i]->FixedUpdate();
}

void GameManager::Draw()
{
	/* Runs the Draw() for all objects in GameManager.
	Should always run after any UpdateX(); functions. */
	for (GameObject* gameObject : GameManager::gameObjects)
		gameObject->Draw();
	for (InterfaceObject* interfaceObject : GameManager::interfaceObjects)
		interfaceObject->Draw();

	/* Runs the DrawGUI() for all interface objects in GameManager.
	Should always run last, so it draws over everything else. */
	for (InterfaceObject* interfaceObject : GameManager::interfaceObjects)
		interfaceObject->DrawGUI();
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

irr::core::vector3df GameManager::Lerp(irr::core::vector3df value, irr::core::vector3df value2, double blend)
{
	return vector3df(value.X + (blend * (value2.X - value.X)),
		value.Y + (blend * (value2.Y - value.Y)),
		value.Z + (blend * (value2.Z - value.Z)));
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

	ISceneNode* selectedSceneNode =
		GameManager::collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,    // This will be the position of the collision
			hitTriangle,    // This will be the triangle hit in the collision
			GameManager::ID_FLAG_IS_PICKABLE,  // This ensures that only nodes that we have set up to be pickable are considered
			0);          // Check the entire scene (this is actually the implicit default)

	return selectedSceneNode;
}

/* Returns the index of the given tag's position in the given tag list.

Used in deciding whether a GameObject is a prey for a Monster for example by giving
the monster's tag list and the target object's tag, but can be used for anything else as well.*/
int GameManager::FindTagInTagList(std::vector<GameObject::Tag> vectorList, GameObject::Tag listTag)
{
	for (int i = 0; i < vectorList.size(); i++)
	{
		if (vectorList[i] == listTag)
			return i;
	}
	return -1;
}