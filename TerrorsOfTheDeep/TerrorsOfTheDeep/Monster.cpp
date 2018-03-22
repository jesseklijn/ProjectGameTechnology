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

// Checks whether a target can be seen.
bool Monster::IsInSight(irr::core::vector3df* startPosition, irr::core::vector3df* endPosition)
{
	ISceneNode* target = GameManager::PerformRaycast(*startPosition, *endPosition);
	
	if (target)
		return true;
	else
		return false;
}

// Checks whether a player can be seen.
bool Monster::IsInSight(Player* player)
{
	ISceneNode* target = GameManager::PerformRaycast(getPosition(), player->getPosition());

	if (target)
		return true;
	else
		return false;
}