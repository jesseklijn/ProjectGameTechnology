#include "Dolphin.h"


Dolphin::Dolphin(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: Critter(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	rotationLerp = 0.001;
	fleeSpeedMultiplier = 3.5f;
	moveSpeed = idleSpeed;
	fleeSpeed = idleSpeed * fleeSpeedMultiplier;
}


Dolphin::~Dolphin()
{

}
