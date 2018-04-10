#pragma once
#include "GameObject.h"
#pragma once
#include "Player.h"
#pragma once
#include <vector>

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
	float idlingRange = 2500.0f;
	float detectionRange = 15000.0f;
	float attackRange = 100.0f;
	float seekTime = 10.0f * 1000.0f;
	float idlePositionTime = 10.0f * 1000.0f;	
	float targetDistance, chaseSpeed, chaseSpeedMultiplier, seekTimer = 0.0f, idlePositionTimer = 0.0f;

	bool rotatesSmoothly = true;
	double rotationLerp = 0.001;
	float agilityFactor = 4.0f;

	bool canSeeTarget = false;
	bool canMove = true;
	bool canAnimate = true;

	irr::core::vector3df currentPosition, targetPosition, moveDirection, moveDirectionTarget;
	std::vector<DynamicUpdater::Tag> targetTags;

	// Main monster state
	enum State { IDLE, CHASING, ATTACKING, SEEKING };
	State state = IDLE;
	State statePrevious = state;

	// Functions
	virtual void Update();
	virtual GameObject* GetTarget();
	virtual bool IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition);
	virtual bool IsInSight(GameObject* targetObject);

private:
	std::vector<GameObject*> tempTargets;
};