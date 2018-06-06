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

EventManager GameManager::eventManager;
#pragma region Core Irrlicht Components
// Initialize Irrlicht device
irr::IrrlichtDevice* GameManager::device =
	createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1920, 1080), 64,
		false, true, false, &eventManager);

// Initialize Irrlicht components
irr::video::IVideoDriver* GameManager::driver = GameManager::device->getVideoDriver();
irr::scene::ISceneManager* GameManager::smgr = GameManager::device->getSceneManager();
irr::gui::IGUIEnvironment* GameManager::guienv = GameManager::device->getGUIEnvironment();
irr::scene::ISceneCollisionManager* GameManager::collMan = GameManager::smgr->getSceneCollisionManager();
irr::gui::IGUIFont* GameManager::font = GameManager::guienv->getFont("../media/UI/Fonts/fontStandard.xml");
#pragma endregion

#pragma region Raycasting
line3d<f32> ray;
vector3df intersection;
triangle3df hitTriangle;
#pragma endregion

#pragma region Variables
std::vector<GameObject*> GameManager::gameObjects;
std::vector<InterfaceObject*> GameManager::interfaceObjects;

GameObject* GameManager::levelMonster = nullptr;
Player* GameManager::levelPlayer = nullptr;

// Timing
float GameManager::deltaTime = 0.0;
float GameManager::deltaTimeMS = 0.0;
float GameManager::deltaTimeFixed = 0.0;
float GameManager::deltaTimeFixedMS = 0.0f;
float GameManager::time = 0.0;
float GameManager::fixedTimeStep = 60.0f;
float GameManager::gameSpeed = 1.0f;

float GameManager::creatureStateRange = 5000.0f;
bool GameManager::gameOver = false;
const irr::core::dimension2du& GameManager::screenDimensions = GameManager::driver->getScreenSize();

/* Seed the random number generator, so we don't end up with
the same random numbers on every run */
int GameManager::gameSeed = 0;

// World dimensions
const int GameManager::WORLD_RADIUS_X = 12000.0f;
const int GameManager::WORLD_RADIUS_Y = 12000.0f;
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
#pragma endregion

// Constructor
GameManager::GameManager()
{
	// NOTE: if EFT_FOG_EXP / EFT_FOG_EXP2, distances don't matter, only density!
	GameManager::driver->setFog(SColor(1, 10, 10, 25), EFT_FOG_EXP2, 0.0f, 5000.0f, 0.0003f);
	GameManager::guienv->getSkin()->setFont(GameManager::font);
	GameManager::smgr->setShadowColor(SColor(100, 0, 0, 0));
	
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

	// Update our input before any other Update() runs on anything
	eventManager.Update();

	/* Runs the Update() for all objects in GameManager.
	Used for basic updates per frame. */
	for (GameObject* gameObject : GameManager::gameObjects)
		if (gameObject != nullptr)
			gameObject->Update();
	for (InterfaceObject* interfaceObject : GameManager::interfaceObjects)
		if (interfaceObject != nullptr)
			interfaceObject->Update();
		
	/* Runs the FixedUpdate() for all objects in GameManager.
	Used for fixed updates at specific timestep intervals, ideally for physics updates. */
	if (fixedTime >= 1.0f / GameManager::fixedTimeStep)
	{
		/* Since it's practically impossible that we land EXACTLY on our timestep with
		fixedTime, we calculate a correction of how many ms we went over our timestep. */
		fixedCorrection = (fixedTime - 1.0f / GameManager::fixedTimeStep);
		GameManager::FixedUpdate();
	}

	// Clean up object lists
	listCleanupTimer = (listCleanupTimer > 0) ? GameManager::Clamp(listCleanupTimer - GameManager::deltaTimeMS, 0.0f, listCleanupTime) : -1.0f;
	if (listCleanupTimer == 0.0f)
	{
		for (int goIndex = 0; goIndex < GameManager::gameObjects.size(); goIndex++)
		{
			if (GameManager::gameObjects[goIndex] == nullptr)
			{
				GameManager::gameObjects.erase(GameManager::gameObjects.begin() + goIndex);
				goIndex--;
			}
		}
		for (int ioIndex = 0; ioIndex < GameManager::interfaceObjects.size(); ioIndex++)
		{
			if (GameManager::interfaceObjects[ioIndex] == nullptr)
			{
				GameManager::interfaceObjects.erase(GameManager::interfaceObjects.begin() + ioIndex);
				ioIndex--;
			}
		}
		listCleanupTimer = listCleanupTime;
	}
}

/* Runs similar to Update();, but after a predetermined timestep. */
void GameManager::FixedUpdate()
{
	fixedTime = 0.0f;

	// Forces the fixed delta times on our timestep, but we add the correction we calculated earlier
	GameManager::deltaTimeFixed = (1.0f / GameManager::fixedTimeStep + fixedCorrection) * GameManager::gameSpeed;
	GameManager::deltaTimeFixedMS = GameManager::deltaTimeFixed * 1000.0f * GameManager::gameSpeed;

	// Runs the FixedUpdate() for all GameObjects in GameManager::gameObjects.
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj != nullptr)
			gameObj->FixedUpdate();
}

void GameManager::Draw()
{
	/* Runs the Draw() for all objects in GameManager.
	Should always run after any UpdateX(); functions. */
	for (GameObject* gameObject : GameManager::gameObjects)
		if (gameObject != nullptr)
			gameObject->Draw();
	for (InterfaceObject* interfaceObject : GameManager::interfaceObjects)
		if (interfaceObject != nullptr)
			interfaceObject->Draw();

	/* Runs the DrawGUI() for all interface objects in GameManager.
	Should always run last, so it draws over everything else. */
	for (InterfaceObject* interfaceObject : GameManager::interfaceObjects)
		if (interfaceObject != nullptr)
			interfaceObject->DrawGUI();
}

// Returns the smallest of the two values.
float GameManager::Min(float value, float value2)
{
	return value <= value2 ? value : value2;
}

// Returns the largest of the two values
float GameManager::Max(float value, float value2)
{
	return value > value2 ? value : value2;
}

// Limits the given value between a min and max value.
float GameManager::Clamp(float value, float minValue, float maxValue)
{
	return GameManager::Max(minValue, GameManager::Min(value, maxValue));
}

// Interpolates between the first and second value with a given blending factor.
float GameManager::Lerp(float value, float value2, float blend)
{
	return value + blend * (value2 - value);
}

// Interpolates between the first and second vector with a given blending factor.
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
ISceneNode* GameManager::PerformRaycast(vector3df startPosition, vector3df endPosition, irr::s32 id)
{
	ray.start = startPosition;
	ray.end = endPosition;

	ISceneNode* selectedSceneNode =
		GameManager::collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,	// This will be the position of the collision
			hitTriangle,	// This will be the triangle hit in the collision
			id,				// This ensures that only nodes that we have set up to be pickable are considered
			0);	// Check the entire scene (this is actually the implicit default)

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
