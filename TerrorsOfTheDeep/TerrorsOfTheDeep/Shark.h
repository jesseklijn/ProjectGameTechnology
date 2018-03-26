#pragma once
#include "Monster.h"
#pragma once
#include <iostream>
#pragma once
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
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~Shark();

	// Variables
	GameObject* target;
	float chaseSpeedMultiplier = 1.25f;
	float detectionRange = 10000.0f;
	float attackRange = 100.0f;
	int idlingRange = 2500;

	float seekTime = 10.0f * 1000.0f;
	float idlePositionTime = 10.0f * 1000.0f;

	bool canSeeTarget = false;
	bool canMove = true;

	// Main shark state
	enum State { IDLE, CHASING, ATTACKING, SEEKING };
	State state = CHASING;


	// Functions
	virtual void Update();
	void OnStateSwitch();

private:
	// Variables
	State statePrevious;
	float targetDistance, chaseSpeed, seekTimer = 0.0f, idlePositionTimer = 0.0f;
	irr::core::vector3df currentPosition, targetPosition, moveDirection;

	// Debug
	std::vector<std::string> stateNames;
};

