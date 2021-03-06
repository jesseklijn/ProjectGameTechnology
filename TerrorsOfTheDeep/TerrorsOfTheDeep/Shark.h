#pragma once
#include "Monster.h"
#include "GameManager.h"
#include <iostream>
#include <vector>

/* Our first Monster: the Shark!
WARNING: Has an appetite for Player classes!

Derives from Monster that has general Monster traits*/
class Shark : public Monster
{
public:
	// Constructor and destructor
	Shark(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0, 
		bool detectCollision = true, bool castsShadows = false);
	~Shark();
};

