#pragma once
#include "GameObject.h"

// Defines a monster, as in a boss enemy.
class Monster : public GameObject
{
public:
	// Constructor and deconstructor
	Monster(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0)
		: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture) {}
	~Monster();

	// Functions
	virtual void Update();
	virtual bool IsInSight(irr::core::vector3df* startPosition, irr::core::vector3df* endPosition);
};