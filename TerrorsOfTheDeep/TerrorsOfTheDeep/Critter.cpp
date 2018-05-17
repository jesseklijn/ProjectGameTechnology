#include "Critter.h"


Critter::Critter(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, 
	bool detectCollision, bool castsShadows)
	: Creature(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision, castsShadows)
{
	canAttack = false;
	canFlee = true;

	stateUpdateTimeDefault = 1.0 * 1000.0f;
	stateUpdateTime = stateUpdateTimeDefault;
	rotationLerp = 0.0008;
	moveSpeed = idleSpeed;	
	fleeSpeed = idleSpeed * fleeSpeedMultiplier;
}


Critter::~Critter()
{

}