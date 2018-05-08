#pragma once
#include "GameObject.h"
#include "Player.h"
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
	GameObject* targetFleeingFrom = nullptr;
	double chaseDetectionRange = INFINITY;
	float attackRange = 500.0f;
	float attackCooldownTimeMin = 5.0f * 1000.0f;
	float attackCooldownTimeMax = 15.0f * 1000.0f;
	float attackCooldownTimer = 0.0f;
	float seekTime = 3.0f * 1000.0f;
	float seekTimer = 0.0f;
	float agilityFactor = 10.0f;
	float agilityDistance = 6000.0f, agility;
	float targetDistance, targetDistanceLast;

	float stateUpdateTimeDefault = 1.0f * 1000.0f;
	float stateUpdateTime = stateUpdateTimeDefault;
	float stateUpdateTimer = 0.0f;

	float idleSpeed = 125.0f;
	float chaseSpeed, fleeSpeed;
	float chaseSpeedMultiplier = 2.0f;
	float fleeSpeedMultiplier = 4.5f;
	int idlingAngle = 60;

	std::vector<GameObject::Tag> targetTags;
	irr::core::vector3df currentPosition, targetPosition, moveDirection, moveDirectionTarget;

	float fleeingDetectionRange = 3000.0f;
	float fleeingTime = 4.0f * 1000.0f;
	float fleeingTimer = 0.0f;
	float fleeingPositionTime = 0.5f * 1000.0f;
	float fleeingPositionTimer = 0.0f;
	int fleeingAngle = 90;

	double rotationLerp = 0.0008;

	// Debug
	std::vector<std::string> stateNames;

	// Animation
	std::vector<irr::video::S3DVertex> baseVertexPositions;


	// Main creature states
	enum State { NONE, IDLE, CHASING, ATTACKING, SEEKING, FLEEING };
	State state = NONE;
	State statePrevious = state;


	// Functions
	virtual void Update();
	virtual void UpdateState();
	virtual void ExecuteState();
	virtual void Move();
	void OnStateSwitch();
};