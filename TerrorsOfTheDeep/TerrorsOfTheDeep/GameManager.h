#pragma region Includes
#pragma once
#include <vector>
#include "GameObject.h"
#include "InterfaceObject.h"
#include "EventManager.h"
#include <iostream>
#include "irrlicht.h"
#include "Player.h"
#pragma endregion

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion


/**
* Class responsible for managing the overall game.
* 
* Holds global components and initializes the core Irrlicht engine. Also keeps 
* track of GameObjects and InterfaceObjects that are added to the GameManager's
* vector<GameObject*> gameObjects or vector<InterfaceObject*> interfaceObjects
* lists, provides search functions for these lists and adds basic math functionality.
* 
* When adding an object to either list, the GameManager will internally
* call the following function structure for each object (in the following order):
*
* - Update(); - Ran every game frame
* - FixedUpdate(); - Ran every fixed timestep, used for physics
* - Draw(); - Ran every game frame, used for drawing something
* in the world.
* - DrawGUI(); - Ran every game frame, used for drawing something
* on a GUI / game interface level.
*
* All positional and conditional behaviour should be executed first, which is
* why Update(); and FixedUpdate(); come first. Drawing should always be done
* after said updates, to prevent jitter/lag of one frame where drawing is done
* on the previous coordinates instead of the actual position of objects at the
* end of the main gameloop. That's why Draw(); and DrawGUI(); come next.
*
* NOTE: FixedUpdate(); is framerate independent with a fixed timestep, so
* it won't run every frame necessarily. The default timestep is 60 frames, which
* means that if you were running the game at 300 FPS, FixedUpdate(); would be
* called every fifth frame. It's also important to note that it comes with its
* own seperate delta timing and corrects an additional timing offset inbetween
* calling frames. It's used for physics, so only GameObjects benefit from it at 
* the moment. InterfaceObjects don't use it as it is. See GameManager::FixedUpdate(); 
* for more details.
*/
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

	static int gameSeed;

	// A list of all X or derived classes of X in the scene
	static std::vector<GameObject*> gameObjects;
	static std::vector<InterfaceObject*> interfaceObjects;

	// Delta timing / Time
	static float deltaTime, deltaTimeMS, deltaTimeFixed, deltaTimeFixedMS, fixedTimeStep, time;
	static const int WORLD_RADIUS_X, WORLD_RADIUS_Y, WORLD_RADIUS_Z;

	static const irr::core::dimension2du& screenDimensions;
	static bool gameOver;

	static bool keyPickedUp;
	static bool escaped;
	static bool hasDied;
	static float creatureStateRange;
	static float gameSpeed;	

	float listCleanupTime = 3.0f * 1000.0f;
	float listCleanupTimer = 0.0f;

	static GameObject* levelMonster;
	static Player* levelPlayer;

	// Scenery generation
	static int critterCount, shipCount, rockCount, ruinsCount, coralCount, plantCount, skullCount;

	// Functions
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void Draw();

	static scene::ISceneNode* PerformRaycast(core::vector3df startPosition, core::vector3df endPosition, irr::s32 id = 0);
	static int FindTagInTagList(std::vector<GameObject::Tag> vectorList, GameObject::Tag listTag);

	/**
	* Template functions for list interactions.
	* 
	* Because we now have GameObjects as well as InterfaceObjects, we'd have to write double functions for both classes.
	* Polymorphism is a way of dealing with this, but we'd still have to write multiple cases of the same function with
	* different type parameters. 
	* 
	* With a template we can use the same function for a variable type parameter. This saves a lot of work, especially
	* if we decided to add more classes at the same level of GameObject or InterfaceObject.
	* 
	* Their implementation is inline due to processing overhead. AI systems such as Monsters and Critters tend to call
	* these regularly to find targets.
	*/
	template <class T> static int FindIndexInList(T* object, std::vector<T*> targetList);
	template <class T> static T* FindObjectWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList);
	template <class T> static std::vector<T*> FindObjectsWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList);
	template <class T> static std::vector<T*> FindObjectsWithTags(std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList);
	template <class T> static T* FindNearestObjectWithTag(T* origin, DynamicUpdater::Tag name, std::vector<T*> objectList, 
		double detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindNearestObjectWithTags(T* origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, 
		float detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindFurthestObjectWithTag(T* origin, DynamicUpdater::Tag name, std::vector<T*> objectList, 
		double detectionRange = INFINITY, bool visibilityCheck = false);
	template <class T> static T* FindFurthestObjectWithTags(T* origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, 
		double detectionRange = INFINITY, bool visibilityCheck = false);

	/**
	* Quick implementation of certain mathematical functions that were needed throughout the game
	* and did not seem to be easily accessible through generic math libraries.
	*/
	static float Min(float value, float value2);
	static float Max(float value, float value2);
	static float Clamp(float value, float minValue, float maxValue);
	static float Lerp(float value, float value2, float blend);
	static irr::core::vector3df Lerp(irr::core::vector3df value, irr::core::vector3df value2, double blend);

private:
	float fixedTime = 0.0f, fixedCorrection;
};

#pragma region Template functions
/* Finds the index of the given object in the given list. Returns -1 if no valid
index was found. */
template<class T>
inline int GameManager::FindIndexInList(T * object, std::vector<T*> targetList)
{
	if (object == nullptr)
		return -1;

	for (int i = 0; i < targetList.size(); i++)
		if (targetList[i] != nullptr && targetList[i] == object)
			return i;
	return -1;
}

/** 
* Returns the first object that satisfies the given tag or a null pointer if no
* object was found.
*/
template<class T>
inline T* GameManager::FindObjectWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList)
{
	for (T* gameObj : objectList)
		if (gameObj != nullptr && gameObj->GetTag() == tag)
			return gameObj;
	return nullptr;
}

// Finds all GameObjects that satisfy the given tag.
template<class T>
inline std::vector<T*> GameManager::FindObjectsWithTag(DynamicUpdater::Tag tag, std::vector<T*> objectList)
{
	std::vector<T*> objs;
	for (T* gameObj : objectList)
		if (gameObj != nullptr && gameObj->GetTag() == tag)
			objs.push_back(gameObj);
	return objs;
}

/**
* Returns a list of objects that satisfy the given tag(s) or an empty list if no
* objects were found.
*/
template<class T>
inline std::vector<T*> GameManager::FindObjectsWithTags(std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList)
{
	std::vector<GameObject*> objs;
	for (GameObject* gameObj : objectList)
		if (gameObj != nullptr && GameManager::FindTagInTagList(tagList, gameObj->tag) != -1)
			objs.push_back(gameObj);
	return objs;
}

/** 
* Finds the nearest object, from another object's position, that satisfies the given tag.
* Optionally a max detection range and a visibility check can be enabled for more specific searches.
*/
template<class T>
inline T * GameManager::FindNearestObjectWithTag(T * origin, DynamicUpdater::Tag tag, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != nullptr && gameObj != origin && gameObj->GetTag() == tag)
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
inline T * GameManager::FindNearestObjectWithTags(T * origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, float detectionRange, bool visibilityCheck)
{
	float closestDistance = INFINITY, currentDistance;
	GameObject* closestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != nullptr && gameObj != origin && GameManager::FindTagInTagList(tagList, gameObj->GetTag()))
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
inline T * GameManager::FindFurthestObjectWithTag(T * origin, DynamicUpdater::Tag name, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != nullptr && gameObj != origin && gameObj->GetTag() == name)
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
inline T * GameManager::FindFurthestObjectWithTags(T * origin, std::vector<DynamicUpdater::Tag> tagList, std::vector<T*> objectList, double detectionRange, bool visibilityCheck)
{
	float furthestDistance = 0.0f, currentDistance;
	GameObject* furthestObject = nullptr;
	for (GameObject* gameObj : objectList)
	{
		if (gameObj != nullptr && gameObj != origin && GameManager::FindTagInTagList(tagList, gameObj->GetTag()))
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
