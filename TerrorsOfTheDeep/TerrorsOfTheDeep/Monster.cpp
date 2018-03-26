#pragma once
#include "Monster.h"
#pragma once
#include "GameManager.h"

// Constructor
Monster::Monster(const irr::core::vector3df* startPosition,
					const irr::core::vector3df* startScale,
					const irr::core::vector3df* startRotation,
					irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
					irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
					: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{

}

// Destructor
Monster::~Monster()
{

}

// Overridden (from GameObject) Update-loop
void Monster::Update()
{
	// Run the Update() of our base class
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