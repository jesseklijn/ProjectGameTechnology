#pragma once
#include "Monster.h"
#pragma once
#include "GameManager.h"

float xSp = 0.0;

// Destructor
Monster::~Monster()
{

}

void Monster::Update()
{
	xSp -= 0.01;
	setPosition(irr::core::vector3df(xSp, 0, 0));
	mesh->setPosition(getPosition());
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