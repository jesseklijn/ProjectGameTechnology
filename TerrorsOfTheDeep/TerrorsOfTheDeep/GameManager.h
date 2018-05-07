#pragma region Includes
#pragma once
#include <vector>
#include "GameObject.h"
#include "InterfaceObject.h"
#include "EventManager.h"
#include <iostream>
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


/* Class responsible for managing the overall game.
Makes use of state tracking and switching.
Runs Update() and Draw() calls for every GameObject. */
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
		ID_IS_NOT_PICKABLE = 0,

		/* Use this flag in ISceneNode IDs to indicate that the 
		scene node can be picked by ray selection.*/ 
		ID_FLAG_IS_PICKABLE = 1 << 0,

		/* Use this flag in ISceneNode IDs to indicate that the 
		scene node can be highlighted.*/ 
		ID_FLAG_IS_HIGHLIGHTABLE = 1 << 1
	};
	#pragma endregion

	GameManager();
	~GameManager();

	// Event receiver
	static EventManager eventManager;

	// A list of all X or derived classes of X in the scene
	static std::vector<GameObject*> gameObjects;
	static std::vector<InterfaceObject*> interfaceObjects;

	// Delta timing / Time
	static float deltaTime, deltaTimeMS, deltaTimeFixed, deltaTimeFixedMS, fixedTimeStep, time;
	static const int WORLD_RADIUS_X, WORLD_RADIUS_Y, WORLD_RADIUS_Z;

	static const irr::core::dimension2du& screenDimensions;

	static bool keyPickedUp;
	static bool escaped;
	static bool hasDied;
	static float creatureStateRange;
	static float gameSpeed;

	// Scenery generation
	static int critterCount, shipCount, rockCount, ruinsCount, coralCount, plantCount, skullCount;

	// Functions
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Draw();

	static scene::ISceneNode* PerformRaycast(core::vector3df startPosition, core::vector3df endPosition);
	static int FindTagInTagList(std::vector<GameObject::Tag> vectorList, GameObject::Tag listTag);

	/* NOTE: These are now template functions for variable type parameters.

	Because we now have GameObjects as well as InterfaceObjects, we'd have to write double functions for both classes.
	With a template this can be varied, but template functions must be implemented in the same header file as the class
	that holds the functions, so you can find their implementation below this class. */
	template <class T> static int FindIndexInList(T* object, std::vector<T*> targetList);
	template <class T> static T* FindGameObjectWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList);
	template <class T> static std::vector<T*> FindGameObjectsWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList);
	template <class T> static std::vector<T*> FindGameObjectsWithTags(std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList);
	template <class T> static T* FindNearestGameObjectWithTag(T* origin, DynamicUpdater::Tag name, std::vector<T*> objectList, double detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindNearestGameObjectWithTags(T* origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, float detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindFurthestGameObjectWithTag(T* origin, DynamicUpdater::Tag name, std::vector<T*> objectList, double detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindFurthestGameObjectWithTags(T* origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, double detectionRange = INFINITY, bool visibilityCheck = false);

	static float Min(float value, float value2);
	static float Max(float value, float value2);
	static float Clamp(float value, float minValue, float maxValue);
	static float Lerp(float value, float value2, float blend);
	static irr::core::vector3df Lerp(irr::core::vector3df value, irr::core::vector3df value2, double blend);

private:
	float fixedTime = 0.0f, fixedCorrection;
};

#pragma region Template functions
template<class T>
inline int GameManager::FindIndexInList(T * object, std::vector<T*> targetList)
{
	for (int i = 0; i < targetList.size(); i++)
		if (targetList[i] == object)
			return i;
	return -1;
}

template<class T>
inline T* GameManager::FindGameObjectWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList)
{
	for (T* gameObj : objectList)
		if (gameObj->GetTag() == tag)
			return gameObj;
	return nullptr;
}

// Finds all GameObjects that satisfy the given tag.
template<class T>
inline std::vector<T*> GameManager::FindGameObjectsWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList)
{
	std::vector<T*> objs;
	for (T* gameObj : objectList)
		if (gameObj->GetTag() == tag)
			objs.push_back(gameObj);
	return objs;
}

// Finds all GameObjects that satisfy the given tag list.
template<class T>
inline std::vector<T*> GameManager::FindGameObjectsWithTags(std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList)
{
	std::vector<GameObject*> objs;
	for (GameObject* gameObj : objectList)
		if (GameManager::FindTagInTagList(tagList, gameObj->tag) != -1)
			objs.push_back(gameObj);
	return objs;
}

/* Finds the nearest GameObject, from another GameObject's position, that satisfies the given tag.
Optionally a max detection range and a visibility check can be enabled for more specific searches. */
template<class T>
inline T * GameManager::FindNearestGameObjectWithTag(T * origin, DynamicUpdater::Tag name, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != origin && gameObj->GetTag() == name)
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
template<class T>
inline T * GameManager::FindNearestGameObjectWithTags(T * origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, float detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : objectList)
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
template<class T>
inline T * GameManager::FindFurthestGameObjectWithTag(T * origin, DynamicUpdater::Tag name, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != origin && gameObj->GetTag() == name)
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
template<class T>
inline T * GameManager::FindFurthestGameObjectWithTags(T * origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : objectList)
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
#pragma endregion