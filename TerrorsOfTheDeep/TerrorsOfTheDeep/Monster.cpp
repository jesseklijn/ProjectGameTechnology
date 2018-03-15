#include "Monster.h"

#pragma once
#include "GameManager.h"

Monster::Monster()
{
	bool foundTarget = IsInSight();
	const wchar_t* result;

	if (foundTarget)
		result = L"Raycast: Yes";
	else
		result = L"Raycast: No";

	GameManager::device->setWindowCaption(result);
}

Monster::~Monster()
{
}

bool Monster::IsInSight()
{
	ISceneNode* target = GameManager::PerformRaycast(GameManager::camera->getPosition(), 
														GameManager::camera->getPosition() + (GameManager::camera->getTarget() - GameManager::camera->getPosition()).normalize() * 1000.0);
	
	if (target)
		return true;
	else
		return false;
}