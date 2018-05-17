#pragma once
#include "Critter.h"

class Fish : public GameObject
{
public:
	// Constructor and deconstructor
	Fish(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0, 
		bool detectCollision = true, bool castsShadows = false);
	~Fish();
	int fishID = 0;
	float movementSpeed = 2;
	float rotationSpeed = 1;
	float neighbourDistance = 25;
	float avoidDistance = 10;
	irr::core::vector3df direction;

	void Move();
	void ApplyFlocking(std::vector<Fish*>, irr::core::vector3df goalPosition);
	static float CalculateDistance(irr::core::vector3df A, irr::core::vector3df B);

};