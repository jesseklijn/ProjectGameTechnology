#pragma once
#include "GameObject.h"
#include <vector>
#include <iostream>
#include "Fish.h"
#include <random>

class FlockingEntity :
	public GameObject
{
public:
	FlockingEntity(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~FlockingEntity();
	virtual void Update();
	std::vector<Fish*> flockOfFishes;
	irr::core::vector3df spawnRadius = irr::core::vector3df(10,50,10);
	irr::core::vector3df fishSizeOffset = irr::core::vector3df(0.5F, 0.5F, 0.5F);
	int amountOfFish = 200, baseFishSpeed = 1;
	
	void SpawnFish();
};

