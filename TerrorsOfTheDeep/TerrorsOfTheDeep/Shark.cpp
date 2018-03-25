#pragma once
#include "Shark.h"
#pragma once
#include "GameManager.h"

// Constructor
Shark::Shark(const irr::core::vector3df* startPosition,
				const irr::core::vector3df* startScale,
				const irr::core::vector3df* startRotation,
				irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
				irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
				: Monster(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{
	moveSpeed = 0.5;
	chaseSpeed = moveSpeed * chaseSpeedMultiplier;
}

// Destructor
Shark::~Shark()
{

}

void Shark::OnStateSwitch()
{
	idlePositionTimer = 0.0f;
	seekTimer = 0.0f;
}

/* Overridden (from GameObject - Monster)

Main player chasing code for vertical slice. Since the player
is parented to the camera, the main camera's position is used.*/
void Shark::Update()
{
	// Update base
	GameObject::Update();

	// TODO: Check if player is in sight with raycasting support
	// Monster::IsInSight();

	/* Determine core state
	Set defaults and gather basic shark information*/
	statePrevious = state;
	state = Idle;
	currentPosition = getPosition();

	/* Get our potential target and target information
	TODO: Use the player as target in case camera will be seperated from the player?*/
	target = GameManager::smgr->getActiveCamera();
	targetDistance = (target->getPosition() - getPosition()).getLength();

	// If target can potentially be spotted
	if (targetDistance < detectionRange)
	{
		/* Switch to Chasing
		TODO: Implement raycast obstruction checks here before going further*/
		state = Chasing;

		// But switch to Attacking if we're close enough to attack the player
		if (targetDistance < attackRange)
			state = Attacking;
	}
	else
	{
		if (seekTimer > 0.0f)
			state = Seeking;
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
		case Idle:
		{
			idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
			if (idlePositionTimer <= 0.0f)
			{
				targetPosition = target->getPosition();/*vector3df(rand() % (idlingRange * 2) - idlingRange,
															rand() % (idlingRange * 2) - idlingRange,
															rand() % (idlingRange * 2) - idlingRange);*/
				idlePositionTimer = idlePositionTime;
			}

			/* Add a vector of length moveSpeed in the direction towards our target position*/
			moveDirection = (targetPosition - currentPosition).normalize();
			GameObject::Move(moveSpeed, moveDirection, true);
		} break;

		// Chase the player/target
		case Chasing:
		{
			/* Add a vector of length moveSpeed in the direction towards our player*/
			targetPosition = target->getPosition() + vector3df(0, -10, 0);
			moveDirection = (targetPosition - currentPosition).normalize();
			GameObject::Move(moveSpeed, moveDirection, true);
			
			// Add the same displacement to our mesh if we have one
			if (mesh)
			{		
				mesh->setPosition(getPosition());
				mesh->setRotation(moveDirection.getHorizontalAngle());			
			}
			break;
		}

		// Attacking when in range
		case Attacking:
		{
			break;
		}

		// Seeking for a short while if lost target
		case Seeking:
		{
			seekTimer = GameManager::Clamp(seekTimer - GameManager::deltaTimeMS, 0.0f, seekTime);
			if (seekTimer <= 0.0f)
			{
				state = Idle;
				seekTimer = seekTime;
			}
		} break;

		default:
		{
			state = Idle;
		} break;
	}
}
