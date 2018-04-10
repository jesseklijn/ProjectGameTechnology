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
	if (canAnimate && (relatedMesh && relatedTexture))
	{
		
	}

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

// Triggered only when the shark switches states
void Shark::OnStateSwitch()
{
	std::cout << stateNames[(int)state] << std::endl;
}

void Shark::Update()
{
	// Update base
	GameObject::Update();

	// Animation
	if (canAnimate && mesh)
	{

	}

	/* Determine core state
	Set defaults and gather basic shark information*/
	statePrevious = state;
	state = IDLE;
	currentPosition = getPosition();
	canSeeTarget = false;
	targetDistance = INFINITY;

	// Update our timers
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	seekTimer = GameManager::Clamp(seekTimer - GameManager::deltaTimeMS, 0.0f, seekTime);

	// Get our potential target and target information
	target = Monster::GetTarget();
	if (target)
	{
		canSeeTarget = Monster::IsInSight(getAbsolutePosition(), target->getAbsolutePosition());

		if (canSeeTarget)
			targetDistance = (target->getAbsolutePosition() - getAbsolutePosition()).getLength();
	}

	// If target can potentially be spotted
	if (targetDistance < detectionRange)
	{
		if (canSeeTarget) 
		{
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

	// Decide what to do depending on our state
	switch (state)
	{
		// Idling when no target 
		case IDLE:
		{
			// Get a new idle position on timer trigger
			if (idlePositionTimer <= 0.0f)
			{
				targetPosition = vector3df(rand() % (int)idlingRange * 2.0f - (int)idlingRange, 0.0f, rand() % (int)idlingRange * 2.0f - (int)idlingRange);
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

	// State switch detector
	if (statePrevious != state)
	{
		OnStateSwitch();
		statePrevious = state;
	}

	// If we're not at our targetPosition, move the shark
	if (canMove)
	{		
		moveDirectionTarget = targetPosition - currentPosition;
		if (moveDirectionTarget.getLength() >= moveSpeed)
		{
			// Calculate an agility factor that lerps the shark sharper towards the target the closer it gets to it
			float agility = 1.0 + (canSeeTarget ? (-moveDirectionTarget.getLength() + detectionRange) / detectionRange * agilityFactor : 0.0);
			moveDirection = GameManager::Lerp(moveDirection, moveDirectionTarget, (rotationLerp * agility) * GameManager::deltaTime).normalize();

			/* Add a vector of length moveSpeed in the direction towards our target position*/
			Shark::Move((canSeeTarget ? chaseSpeed : moveSpeed) * GameManager::deltaTime, moveDirection, true);
		}
	}
}