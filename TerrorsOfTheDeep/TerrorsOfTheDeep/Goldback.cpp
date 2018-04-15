#include "Goldback.h"


Goldback::Goldback(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: Critter(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	rotationLerp = 0.001;
	moveSpeed = idleSpeed;
	fleeSpeed = idleSpeed * fleeSpeedMultiplier;

	mesh->setMaterialFlag(irr::video::EMF_LIGHTING, false);
}

Goldback::~Goldback()
{

}
