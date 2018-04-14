#pragma once
#include "Creature.h"

// Defines a normal and passive sea creature.
class Critter : public Creature
{
public:
	// Constructor and deconstructor
	Critter(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0, bool detectCollision = true);
	~Critter();
};