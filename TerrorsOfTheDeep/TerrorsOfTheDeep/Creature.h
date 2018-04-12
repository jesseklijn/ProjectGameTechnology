#pragma once
#include "GameObject.h"
#pragma once
#include "Player.h"
#pragma once
#include <vector>

// Defines a non-player creature-like being
class Creature : public GameObject
{
public:
	// Constructor and deconstructor
	Creature(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0, bool detectCollision = true);
	~Creature();


	// Variables
	bool canMove = true;
	bool canAttack = false;
	bool canSeeTarget = false;
	bool canFlee = true;
	bool canAnimate = true;
	bool rotatesSmoothly = true;

	float idlingRange = 2500.0f;
	float idlePositionTime = 10.0f * 1000.0f;
	float idlePositionTimer = 0.0f;

	GameObject* targetAttack = nullptr;
	GameObject* targetFlee = nullptr;
	float chaseDetectionRange = 10000.0f;
	float attackRange = 100.0f;
	float seekTime = 10.0f * 1000.0f;
	float seekTimer = 0.0f;
	float agilityFactor = 10.0f, agility;
	float targetDistance;
	float idleSpeed = 70.0f;
	float chaseSpeed, fleeSpeed;
	float chaseSpeedMultiplier = 2.5f;
	std::vector<GameObject::Tag> targetTags;
	irr::core::vector3df currentPosition, targetPosition, moveDirection, moveDirectionTarget;

	float fleeingDetectionRange = 1000.0f;
	float fleeingTime = 15.0f * 1000.0f;
	float fleeingTimer = 0.0f;

	double rotationLerp = 0.0001;

	// Debug
	std::vector<std::string> stateNames;

	// Animation
	std::vector<irr::video::S3DVertex> baseVertexPositions;


	// Main creature states
	enum State { IDLE, CHASING, ATTACKING, SEEKING, FLEEING };
	State state = IDLE;
	State statePrevious = state;


	// Functions
	virtual void Update();
	virtual void UpdateState();
	virtual void ExecuteState();
	virtual void Move();
	void OnStateSwitch();
};