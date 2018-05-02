#pragma once
#include "Monster.h"
#include "GameManager.h"

// Constructor
Monster::Monster(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: Creature(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	GameObject::setTag(GameObject::MONSTER);

	canAttack = true;
	canFlee = false;

	rotationLerp = 0.0001;
	idleSpeed = 35.0f;
	moveSpeed = idleSpeed;	
	chaseSpeed = idleSpeed * chaseSpeedMultiplier;

	targetTags.push_back(GameObject::PLAYER);
	targetTags.push_back(GameObject::CREATURE);
}

// Destructor
Monster::~Monster()
{

}

/* Finds the player in-game and decides whether or not it is a valid target.
Detection range and a raycast can be enabled for specified checks.

This is a shortcut instead of having to specify extra Player override functionality
in the other GameManager::FindX(); functions, which should be for general usage. */
GameObject* Monster::PlayerCanBeSeen(double detectionRange, bool visibilityCheck)
{
	Player* player = (Player*)GameManager::FindGameObjectWithTag(PLAYER);
	if (player)
	{
		float currentDistance = (player->getAbsolutePosition() - getAbsolutePosition()).getLength();
		if (currentDistance < detectionRange)
		{
			if (!visibilityCheck || Monster::IsInSight(getAbsolutePosition(), player->getAbsolutePosition()))
			{
				return player;
			}
		}
	}
	return nullptr;
}

/* Returns a target based on this monster's tags. Optionally a player detection override, detection range and visibility check can
be enabled for more specific searches. Returns a nullptr if no target was found with the given specifications. */
GameObject* Monster::GetTarget(Monster::TargetPriority priorityMode, bool playerDetectOverride, double detectionRange, bool visibilityCheck)
{
	// If the player should override all other detection
	if (playerDetectOverride)
	{
		// Return the player if he/she's a valid target
		GameObject* player = (Player*)Monster::PlayerCanBeSeen(detectionRange, visibilityCheck);
		if (player != nullptr)
			return player;
	}

	switch (priorityMode)
	{		
		case CLOSEST:
		{
			// Finds the nearest GameObject that satisfies this Monster's prey tag list, regardless of class.
			return GameManager::FindNearestGameObjectWithTags(this, targetTags, detectionRange, visibilityCheck);
			break;
		}

		case FURTHEST:
		{
			// Finds the furthest GameObject that satisfies this Monster's prey tag list, regardless of class.
			return GameManager::FindFurthestGameObjectWithTags(this, targetTags, detectionRange, visibilityCheck);
			break;
		}

		default:
		{
			std::cout << "[ERROR] Invalid priority mode, returning nullptr...";
			return nullptr;
		}
	}
}

// Overridden (from GameObject) Update-loop
void Monster::Update()
{
	/* Run the Update() of our base class.
	Since this is a boss type creature, we have our own Update() in children, so don't inherit Update() from Creature.*/
	GameObject::Update();
}

// Checks whether a target position can be seen.
bool Monster::IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition)
{
	ISceneNode* target = GameManager::PerformRaycast(startPosition, endPosition);
	
	// If I didn't find anything in the way
	if (!target)
		return true;
	else
		return false;
}

// Checks whether a player can be seen.
bool Monster::IsInSight(GameObject* targetObject)
{
	GameObject &tar = *targetObject;

	// If this GameObject has no mesh, return false
	if (!tar.mesh)
		return false;

	// If I hit a triangle of the target
	ISceneNode* target = GameManager::PerformRaycast(getPosition(), tar.getPosition());
	if (target)
		return true;
	else
		return false;
}