#pragma once
#include "Shark.h"
#include "GameManager.h"

// Constructor
Shark::Shark(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, 
	bool detectCollision, bool castsShadows)
	: Monster(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision, castsShadows)
{
	
}

// Destructor
Shark::~Shark()
{

}