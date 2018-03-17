#pragma once
#include "Monster.h"
#pragma once
#include "GameManager.h"

// Constructor
Monster::Monster()
{

}

// Destructor
Monster::~Monster()
{

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