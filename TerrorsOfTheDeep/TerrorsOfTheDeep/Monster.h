#pragma once
#include "GameObject.h"
#pragma once
#include "Player.h"

// Defines a monster, as in a boss enemy.
class Monster : public GameObject
{
public:
	// Constructor and deconstructor
	Monster(const irr::core::vector3df* startPosition,
			const irr::core::vector3df* startScale,
			const irr::core::vector3df* startRotation,
			irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
			irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0, bool detectCollision = true);
	~Monster();

	// Variables
	GameObject* target;
	float chaseSpeedMultiplier = 1.25f;
	float detectionRange = 10000.0f;
	float attackRange = 100.0f;
	int idlingRange = 2500;

	float seekTime = 10.0f * 1000.0f;
	float idlePositionTime = 10.0f * 1000.0f;

	bool rotatesSmoothly = true;
	float rotationLerp = 0.00001;
	bool canSeeTarget = false;
	bool canMove = true;

	float targetDistance, chaseSpeed, seekTimer = 0.0f, idlePositionTimer = 0.0f;
	irr::core::vector3df currentPosition, targetPosition, moveDirection, moveDirectionTarget;

	// Main monster state
	enum State { IDLE, CHASING, ATTACKING, SEEKING };
	State state = IDLE;
	State statePrevious = state;

	// Functions
	virtual void Update();
	virtual bool IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition);
	virtual bool IsInSight(GameObject* targetObject);
};