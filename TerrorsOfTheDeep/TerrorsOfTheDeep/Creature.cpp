#pragma once
#include "Creature.h"
#include "GameManager.h"


// Constructor
Creature::Creature(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	tag = GameObject::CREATURE;

	buoyancyConstant = 9.79;

	idlingRange = GameManager::Min(GameManager::WORLD_RADIUS_X, GameManager::WORLD_RADIUS_Z);
	moveSpeed = idleSpeed;
	chaseSpeed = idleSpeed * chaseSpeedMultiplier;
	fleeSpeed = idleSpeed * fleeSpeedMultiplier;

	/* [DEBUG] Add our states to a locally accessible string array for state debugging prints
	Not used in any other way!*/
	stateNames.push_back("NONE");
	stateNames.push_back("IDLE");
	stateNames.push_back("CHASING");
	stateNames.push_back("ATTACKING");
	stateNames.push_back("SEEKING");
	stateNames.push_back("FLEEING");
}

Creature::~Creature()
{

}

void Creature::OnStateSwitch()
{
	//std::cout << "Creature: " + stateNames[(int)state] << std::endl;
}

void Creature::UpdateState()
{
	/* Determine core state
	Set defaults and gather basic shark information*/
	statePrevious = state;
	state = IDLE;
	currentPosition = getAbsolutePosition();
	canSeeTarget = false;
	targetDistance = INFINITY;
	moveSpeed = idleSpeed;

	// If I'm within base fleeing distance of the closest predator, max out fleeing timer
	if (canFlee)
	{
		targetFleeingFrom = GameManager::FindNearestGameObjectWithTag(this, GameObject::MONSTER, INFINITY, true);
		if (targetFleeingFrom)
		{
			if ((targetFleeingFrom->getAbsolutePosition() - currentPosition).getLength() < fleeingDetectionRange)
				fleeingTimer = fleeingTime;
		}
		if (fleeingTimer > 0.0f)
			state = FLEEING;
	}
}

void Creature::ExecuteState()
{
	// Decide what to do depending on our state
	switch (state)
	{
		case IDLE:
		{
			moveSpeed = idleSpeed;

			// Get a new idle position on timer trigger
			if (idlePositionTimer <= 0.0f)
			{
				// If we're close enough to the world center point, idle normally in 360 degrees
				vector3df toWorldCenter = currentPosition - vector3df(0, 0, 0);
				if (toWorldCenter.getLength() < idlingRange)
					targetPosition = vector3df(rand() % (int)(idlingRange * 2.0f) - (int)idlingRange, 50.0f, rand() % (int)(idlingRange * 2.0f) - (int)idlingRange);
				// Otherwise, generate a direction that generally points back to world center
				else
				{
					float newAngle = GameManager::Clamp(rand() % (idlingAngle * 2), -idlingAngle, idlingAngle);
					vector3df newDirection = toWorldCenter;
					newDirection.rotateXZBy(newAngle, currentPosition);
					targetPosition = currentPosition + newDirection;
				}

				idlePositionTimer = idlePositionTime;
			}

			break;
		}

		case FLEEING:
		{
			moveSpeed = fleeSpeed;

			/* Set a target position away from the closest predator
			This position is randomized to a max fleeingAngle * 2 degree cone in the other direction */
			if (targetFleeingFrom)
			{
				if (fleeingPositionTimer <= 0.0f)
				{
					float newAngle = GameManager::Clamp(rand() % (fleeingAngle * 2), -fleeingAngle, fleeingAngle);
					vector3df newDirection = GameManager::Lerp(currentPosition - targetFleeingFrom->getAbsolutePosition(),
																vector3df(0, 0, 0) - currentPosition, 0.0);

					newDirection.rotateXZBy(newAngle, currentPosition);
					targetPosition = currentPosition + newDirection.normalize() * 5000.0;

					fleeingPositionTimer = fleeingPositionTime;
				}
			}				

			break;
		}
	}

	// State switch detector
	if (statePrevious != state)
	{
		Creature::OnStateSwitch();
		statePrevious = state;
	}
}

void Creature::Move()
{
	// If we're not at our targetPosition, move the shark
	if (canMove)
	{
		moveDirectionTarget = targetPosition - currentPosition;
		if (moveDirectionTarget.getLength() >= moveSpeed)
		{
			// Calculate an agility factor that lerps the shark sharper towards the target the closer it gets to it
			agility = 1.0;
			if (canAttack && moveDirectionTarget.getLength() <= agilityDistance)
				agility += canSeeTarget ? (-moveDirectionTarget.getLength() + agilityDistance) / agilityDistance * agilityFactor : 0.0;

			moveDirection = GameManager::Lerp(moveDirection, moveDirectionTarget, (rotationLerp * agility) * GameManager::deltaTime).normalize();

			/* Add a vector of length moveSpeed in the direction towards our target position*/
			GameObject::Move(moveSpeed * GameManager::deltaTime, moveDirection, true);
		}
	}
}

void Creature::Update()
{
	// Update base
	GameObject::Update();
	
	// Update our timers
	stateUpdateTimer = GameManager::Clamp(stateUpdateTimer - GameManager::deltaTimeMS, 0.0f, stateUpdateTime);
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	fleeingTimer = GameManager::Clamp(fleeingTimer - GameManager::deltaTimeMS, 0.0f, fleeingTime);
	fleeingPositionTimer = GameManager::Clamp(fleeingPositionTimer - GameManager::deltaTimeMS, 0.0f, fleeingPositionTime);

	if (stateUpdateTimer <= 0.0)
	{
		if ((getAbsolutePosition() - GameManager::smgr->getActiveCamera()->getAbsolutePosition()).getLength() < GameManager::creatureStateRange)
			Creature::UpdateState();
		else
			state = IDLE;

		stateUpdateTimer = rand() % (int)stateUpdateTime;
	}
	Creature::ExecuteState();
	Creature::Move();
}
