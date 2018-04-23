#pragma once
#include "GameManager.h"
#pragma once
#include <utility>
#pragma once
#include <algorithm>
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

// Timing
float GameManager::deltaTime = 0.0;
float GameManager::deltaTimeMS = 0.0;
float GameManager::deltaTimeFixed = 0.0;
float GameManager::deltaTimeFixedMS = 0.0;
float GameManager::time = 0.0;
float GameManager::fixedTimeStep = 60.0f;
float GameManager::creatureStateRange = 2500.0f;

// World dimensions
const int GameManager::worldRadiusX = 8000.0f;
const int GameManager::worldRadiusY = 3500.0f;
const int GameManager::worldRadiusZ = 8000.0f;

#pragma endregion

// Constructor
GameManager::GameManager()
{
	// NOTE: if EFT_FOG_EXP / EFT_FOG_EXP2, distances don't matter, only density!
	GameManager::driver->setFog(SColor(1, 0, 0, 25), EFT_FOG_EXP, 0.0f, 5000.0f, 0.0005f);
	GameManager::guienv->getSkin()->setFont(GameManager::device->getGUIEnvironment()->getBuiltInFont());

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

void GameManager::Update()
{
	fixedTime += GameManager::deltaTime;

	// Runs the Update() for all GameObjects in GameManager::gameObjects.
	for (int i = 0; i < GameManager::gameObjects.size(); i++)
		GameManager::gameObjects[i]->Update();
		
	if (fixedTime >= 1.0f / GameManager::fixedTimeStep)
	{
		fixedCorrection = (fixedTime - 1.0f / GameManager::fixedTimeStep);
		GameManager::FixedUpdate();
	}
}

void GameManager::FixedUpdate()
{
	fixedTime = 0.0f;
	GameManager::deltaTimeFixed = 1.0f / GameManager::fixedTimeStep + fixedCorrection;
	GameManager::deltaTimeFixedMS = GameManager::deltaTimeFixed * 1000.0f;

	// Runs the FixedUpdate() for all GameObjects in GameManager::gameObjects.
	for (int i = 0; i < GameManager::gameObjects.size(); i++)
		GameManager::gameObjects[i]->FixedUpdate();
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

irr::core::vector3df GameManager::Lerp(irr::core::vector3df value, irr::core::vector3df value2, double blend)
{
	//vector3df before = vector3df(value.X, value.Y, value.Z);
	vector3df after = vector3df(value.X + (blend * (value2.X - value.X)),
		value.Y + (blend * (value2.Y - value.Y)),
		value.Z + (blend * (value2.Z - value.Z)));

	return after;
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

/* Finds the first GameObject that matches the given tag, in no particular order.
It simply returns the first match found in the GameManager's GameObject list. */
GameObject* GameManager::FindGameObjectWithTag(GameObject::Tag name)
{
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj->GetTag() == name)
			return gameObj;
	return nullptr;
}

// Finds all GameObjects that satisfy the given tag.
std::vector<GameObject*> GameManager::FindGameObjectsWithTag(GameObject::Tag name)
{
	std::vector<GameObject*> objs;
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj->GetTag() == name)
			objs.push_back(gameObj);
	return objs;
}

// Finds all GameObjects that satisfy the given tag list.
std::vector<GameObject*> GameManager::FindGameObjectsWithTags(std::vector<GameObject::Tag> tagList)
{
	std::vector<GameObject*> objs;
	for (GameObject* gameObj : GameManager::gameObjects)
		if (GameManager::FindTagInTagList(tagList, gameObj->tag) != -1)
			objs.push_back(gameObj);
	return objs;
}

/* Finds the nearest GameObject, from another GameObject's position, that satisfies the given tag.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
GameObject* GameManager::FindNearestGameObjectWithTag(GameObject* origin, GameObject::Tag tag, float detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : GameManager::gameObjects)
	{
		if (gameObj != origin && gameObj->GetTag() == tag)
		{
			currentDistance = (gameObj->getAbsolutePosition() - origin->getAbsolutePosition()).getLength();
			if (currentDistance < detectionRange && currentDistance < closestDistance)
			{
				if (!visibilityCheck || !GameManager::PerformRaycast(origin->getAbsolutePosition(), gameObj->getAbsolutePosition()))
				{
					closestDistance = currentDistance;
					closestObject = gameObj;
				}
			}
		}
	}
	return closestObject;
}

/* Finds the nearest GameObject, from another GameObject's position, that satisfies the given tag list.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
GameObject* GameManager::FindNearestGameObjectWithTags(GameObject* origin, std::vector<GameObject::Tag> tagList, float detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : GameManager::gameObjects)
	{
		if (gameObj != origin && GameManager::FindTagInTagList(tagList, gameObj->GetTag()))
		{
			currentDistance = (gameObj->getAbsolutePosition() - origin->getAbsolutePosition()).getLength();
			if (currentDistance < detectionRange && currentDistance < closestDistance)
			{
				if (!visibilityCheck || !GameManager::PerformRaycast(origin->getAbsolutePosition(), gameObj->getAbsolutePosition()))
				{
					closestDistance = currentDistance;
					closestObject = gameObj;
				}
			}
		}
	}
	return closestObject;
}

/* Finds the furthest GameObject, from another GameObject's position, that satisfies the given tag.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
GameObject* GameManager::FindFurthestGameObjectWithTag(GameObject* origin, GameObject::Tag tag, float detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : GameManager::gameObjects)
	{
		if (gameObj != origin && gameObj->GetTag() == tag)
		{
			currentDistance = (gameObj->getAbsolutePosition() - origin->getAbsolutePosition()).getLength();
			if (currentDistance < detectionRange && currentDistance > furthestDistance)
			{
				if (!visibilityCheck || !GameManager::PerformRaycast(origin->getAbsolutePosition(), gameObj->getAbsolutePosition()))
				{
					furthestDistance = currentDistance;
					furthestObject = gameObj;
				}
			}
		}
	}
	return furthestObject;
}

/* Finds the nearest GameObject, from another GameObject's position, that satisfies the given tag list.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
GameObject* GameManager::FindFurthestGameObjectWithTags(GameObject* origin, std::vector<GameObject::Tag> tagList, float detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : GameManager::gameObjects)
	{
		if (gameObj != origin && GameManager::FindTagInTagList(tagList, gameObj->GetTag()))
		{
			currentDistance = (gameObj->getAbsolutePosition() - origin->getAbsolutePosition()).getLength();
			if (currentDistance < detectionRange && currentDistance > furthestDistance)
			{
				if (!visibilityCheck || !GameManager::PerformRaycast(origin->getAbsolutePosition(), gameObj->getAbsolutePosition()))
				{
					furthestDistance = currentDistance;
					furthestObject = gameObj;
				}
			}
		}
	}
	return furthestObject;
}
