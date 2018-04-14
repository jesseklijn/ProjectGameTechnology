#include "Critter.h"


Critter::Critter(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: Creature(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	canAttack = false;
	canFlee = true;

	fleeSpeedMultiplier = 2.0f;
	rotationLerp = 0.0008;
	idleSpeed = 120.0f;
	moveSpeed = idleSpeed;	
	fleeSpeed = idleSpeed * fleeSpeedMultiplier;
}


Critter::~Critter()
{

}