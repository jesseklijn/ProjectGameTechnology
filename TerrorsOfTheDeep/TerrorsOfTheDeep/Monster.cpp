#pragma once
#include "Monster.h"
#pragma once
#include "GameManager.h"

// Constructor
Monster::Monster(const irr::core::vector3df* startPosition,
					const irr::core::vector3df* startScale,
					const irr::core::vector3df* startRotation,
					irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
					irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
					: Creature(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	tag = GameObject::MONSTER;

	canAttack = true;
	canFlee = false;

	rotationLerp = 0.0001;
	idleSpeed = 90.0f;
	moveSpeed = idleSpeed;
	chaseSpeedMultiplier = 2.5f;
	chaseSpeed = idleSpeed * chaseSpeedMultiplier;
	fleeSpeed = chaseSpeed;

	targetTags.push_back(GameObject::PLAYER);
	targetTags.push_back(GameObject::CREATURE);
}

// Destructor
Monster::~Monster()
{

}

// Returns a target based on this monster's tags
GameObject* Monster::GetTarget()
{
	tempTargets.clear();
	for (int i = 0; i < GameManager::gameObjects.size(); i++)
	{
		GameObject::Tag currentTag = GameManager::gameObjects[i]->GetTag();

		// If this object's tag is contained in this monster's prey tag list
		if (GameManager::FindTagInTagList(targetTags, currentTag) != -1)
		{
			canSeeTarget = Monster::IsInSight(getAbsolutePosition(), GameManager::gameObjects[i]->getAbsolutePosition());
			if (canSeeTarget)
			{
				// If the player is part of the valid objects, return the player immediately
				if (currentTag == GameObject::PLAYER)
					return GameManager::gameObjects[i];

				tempTargets.push_back(GameManager::gameObjects[i]);
			}
		}
	}

	// TODO: Priority sorting
	if (tempTargets.size() > 0)
		return tempTargets[0];

	return nullptr;
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