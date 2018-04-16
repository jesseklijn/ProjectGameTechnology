#pragma once
#include "Fish.h"
#pragma once
#include "GameManager.h"


// Constructor
Fish::Fish(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{



}

Fish::~Fish()
{
	
}

void Fish::Update()
{
	// Inherit base class Update
	GameObject::Update();

}

