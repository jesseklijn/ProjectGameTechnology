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
float GameManager::deltaTime = 0.0;
float GameManager::deltaTimeMs = 0.0;
float GameManager::time = 0.0;
const int GameManager::WORLD_RADIUS_X = 5000.0f;
const int GameManager::WORLD_RADIUS_Y = 2000.0f;
const int GameManager::WORLD_RADIUS_Z = 5000.0f;
bool GameManager::keyPickedUp = false;
bool GameManager::escaped = false;
bool GameManager::hasDied = false;
#pragma endregion

// Constructor
GameManager::GameManager()
{
	GameManager::driver->setFog(SColor(0, 0, 0, 0), EFT_FOG_LINEAR, 50.0f, 3000.0f, 0.01f);
	GameManager::guienv->getSkin()->setFont(GameManager::device->getGUIEnvironment()->getBuiltInFont());

	auto playingMesh = GridMesh(
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
= default;

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
	// Runs the Update() for all GameObjects in GameManager::gameObjects.
	for (int i = 0; i < GameManager::gameObjects.size(); ++i)
	{
		GameManager::gameObjects[i]->Update();
	}
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

float GameManager::Clamp(const float value, const float minValue, const float maxValue)
{
	return GameManager::Max(minValue, GameManager::Min(value, maxValue));
}

float GameManager::Lerp(const float value, const float value2, const float blend)
{
	return value + blend * (value2 - value);
}

irr::core::vector3df GameManager::Lerp(const irr::core::vector3df value, const irr::core::vector3df value2, const double blend)
{
	//vector3df before = vector3df(value.X, value.Y, value.Z);
	auto after = vector3df(value.X + (blend * (value2.X - value.X)),
		value.Y + (blend * (value2.Y - value.Y)),
		value.Z + (blend * (value2.Z - value.Z)));

	return after;
}

// Switch to the given GameState.
// TODO: Functionality!
void GameManager::GameStateTransition(GAME_STATE stateToLoad)
{

}

// Cleans up the given state.
// TODO: Functionality!
void GameManager::UnloadGameState(GAME_STATE stateToCleanUp)
{

}

/* Cast a Raycast line between the given start and end positions and return the ISceneNode that was hit.
NOTE: In order for raycast detection, nodes must be flagged as such first by linking a selector to it.

Example:
	selector = GameManager::smgr->createTriangleSelector(<NODE>);
	<NODE>->setTriangleSelector(selector);
	selector->drop();
*/
ISceneNode* GameManager::PerformRaycast(const vector3df& startPosition, const vector3df& endPosition)
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
			GameManager::ID_FLAG_IS_PICKABLE,  // This ensures that only nodes that we have set up to be pickable are considered
			0);          // Check the entire scene (this is actually the implicit default)

	return selectedSceneNode;
}

/* Returns the index of the given tag's position in the given tag list.

Used in deciding whether a GameObject is a prey for a Monster for example by giving
the monster's tag list and the target object's tag, but can be used for anything else as well.*/
int GameManager::FindTagInTagList(std::vector<GameObject::Tag> vectorList, const GameObject::Tag listTag)
{
	for (auto i = 0; i < vectorList.size(); i++)
	{
		if (vectorList[i] == listTag)
			return i;
	}
	return -1;
}

/* Finds the first GameObject that matches the given tag, in no particular order.
It simply returns the first match found in the GameManager's GameObject list. */
GameObject* GameManager::FindGameObjectWithTag(const GameObject::Tag name)
{
	for (auto gameObj : GameManager::gameObjects)
		if (gameObj->GetTag() == name)
			return gameObj;
	return nullptr;
}

// Finds all GameObjects that satisfy the given tag.
std::vector<GameObject*> GameManager::FindGameObjectsWithTag(const GameObject::Tag name)
{
	std::vector<GameObject*> objs;
	for (auto gameObj : GameManager::gameObjects)
		if (gameObj->GetTag() == name)
			objs.push_back(gameObj);
	return objs;
}

// Finds all GameObjects that satisfy the given tag list.
std::vector<GameObject*> GameManager::FindGameObjectsWithTags(const std::vector<GameObject::Tag>& tagList)
{
	std::vector<GameObject*> objs;
	for (auto gameObj : GameManager::gameObjects)
		if (GameManager::FindTagInTagList(tagList, gameObj->tag) != -1)
			objs.push_back(gameObj);
	return objs;
}

/* Finds the nearest GameObject, from another GameObject's position, that satisfies the given tag.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
GameObject* GameManager::FindNearestGameObjectWithTag(GameObject* origin, const GameObject::Tag tag, const float detectionRange, const bool visibilityCheck)
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
GameObject* GameManager::FindNearestGameObjectWithTags(GameObject* origin, const std::vector<GameObject::Tag>& tagList, const float detectionRange, const bool visibilityCheck)
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
GameObject* GameManager::FindFurthestGameObjectWithTag(GameObject* origin, const GameObject::Tag tag, const float detectionRange, const bool visibilityCheck)
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
GameObject* GameManager::FindFurthestGameObjectWithTags(GameObject* origin, const std::vector<GameObject::Tag> tagList, const float detectionRange, const bool visibilityCheck)
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