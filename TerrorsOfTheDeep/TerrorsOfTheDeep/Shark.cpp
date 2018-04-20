#pragma once
#include "Shark.h"
#pragma once
#include "GameManager.h"

// Constructor
Shark::Shark(const irr::core::vector3df* startPosition,
				const irr::core::vector3df* startScale,
				const irr::core::vector3df* startRotation,
				irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
				irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
				: Monster(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	// Shark configuration
	moveSpeed = 0.04;
	chaseSpeed = moveSpeed * chaseSpeedMultiplier;
	chaseSpeedMultiplier = 1.25f;

	detectionRange = 10000.0f;
	attackRange = 100.0f;
	idlingRange = 2500;

	seekTime = 10.0f * 1000.0f;
	idlePositionTime = 10.0f * 1000.0f;

	canSeeTarget = false;
	canMove = true;

	/* [DEBUG] Add our states to a locally accessible string array for state debugging prints
	Not used in any other way!*/
	stateNames.push_back("IDLE");
	stateNames.push_back("CHASING");
	stateNames.push_back("ATTACKING");
	stateNames.push_back("SEEKING");
}

// Destructor
Shark::~Shark()
{

}

void Shark::OnStateSwitch()
{
	std::cout << stateNames[(int)state] << std::endl;
}

/* Overridden (from GameObject - Monster)

Main player chasing code for vertical slice. Since the player
is parented to the camera, the main camera's position is used.*/
void Shark::Update()
{
	// Update base
	GameObject::Update();

	/* Determine core state
	Set defaults and gather basic shark information*/
	statePrevious = state;
	state = IDLE;
	currentPosition = getPosition();

	// Update our timers
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	seekTimer = GameManager::Clamp(seekTimer - GameManager::deltaTimeMS, 0.0f, seekTime);

	/* Get our potential target and target information
	TODO: Use the player as target in case camera will be seperated from the player?*/
	target = GameManager::FindGameObjectWithTag("Player");
	if (target)
	{
		canSeeTarget = Monster::IsInSight(getAbsolutePosition(), target->getAbsolutePosition());
		targetDistance = (target->getPosition() - getPosition()).getLength();
	}
	else
	{
		canSeeTarget = false;
		targetDistance = INFINITY;
	}

	// If target can potentially be spotted
	if (targetDistance < detectionRange)
	{
		if (canSeeTarget) 
		{
			/* Switch to Chasing
			TODO: Implement raycast obstruction checks here before going further*/
			state = CHASING;

			// But switch to Attacking if we're close enough to attack the player
			if (targetDistance < attackRange)
				state = ATTACKING;
		}
	}
	else
	{
		/* Our target can't be spotted.
		Have we seen him recently? Try and seek him out. Otherwise, idle*/
		if (seekTimer > 0.0f)
			state = SEEKING;
	}

	// State switch detector
	if (statePrevious != state)
	{
		OnStateSwitch();
		statePrevious = state;
	}															

	// Decide what to do depending on our state
	switch (state)
	{
		// Idling when no target 
		case IDLE:
		{
			// Get a new idle position on timer trigger
			if (idlePositionTimer <= 0.0f)
			{
				targetPosition = vector3df(rand() % idlingRange * 2.0f - idlingRange, 0.0f, rand() % idlingRange * 2.0f - idlingRange);
				idlePositionTimer = idlePositionTime;
			}

			break;
		}

		// Chase the player/target
		case CHASING:
		{
			// Set our target position on the player
			targetPosition = target->getAbsolutePosition() + vector3df(0, -12, 0);

			// Max out our seek timer until we leave our state, so it ticks down when our target is lost
			seekTimer = seekTime;

			break;
		}

		// Attacking when in range
		case ATTACKING:
		{
			// RawrXD
			break;
		}

		// Seeking for a short while if lost target
		case SEEKING:
		{			
			// When our seeking time runs out, switch back to IDLE
			if (seekTimer <= 0.0f)
			{
				state = IDLE;
				seekTimer = seekTime;
			}

			break;
		}

		// If invalid state, switch to default IDLE state and report to console
		default:
		{
			state = IDLE;
			std::cout << "[ERROR] Invalid state, defaulting to IDLE..." << std::endl;
			break;
		}
	}

	// If we're not at our targetPosition, move the shark
	if (canMove)
	{
		/* Add a vector of length moveSpeed in the direction towards our target position*/
		moveDirectionTarget = targetPosition - currentPosition;
		if (moveDirectionTarget.getLength() >= moveSpeed)
		{
			moveDirection = GameManager::Lerp(moveDirection, moveDirectionTarget, rotationLerp).normalize();
			Shark::Move(moveSpeed, moveDirection, true);
		}
	}
}