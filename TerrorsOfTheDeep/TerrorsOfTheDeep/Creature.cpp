#pragma once
#include "Creature.h"
#pragma once
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

	moveSpeed = idleSpeed;
	chaseSpeed = idleSpeed * chaseSpeedMultiplier;
	fleeSpeed = chaseSpeed;

	/* [DEBUG] Add our states to a locally accessible string array for state debugging prints
	Not used in any other way!*/
	stateNames.push_back("IDLE");
	stateNames.push_back("CHASING");
	stateNames.push_back("ATTACKING");
	stateNames.push_back("SEEKING");
	stateNames.push_back("FLEEING");
}

Creature::~Creature()
{

}

// Triggered only when the creature switches states
void Creature::OnStateSwitch()
{
	std::cout << stateNames[(int)state] << std::endl;
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

	// Update our timers
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	fleeingTimer = GameManager::Clamp(fleeingTimer - GameManager::deltaTimeMS, 0.0f, fleeingTime);

	// If I'm within base fleeing distance of the closest predator, max out fleeing timer
	if (canFlee)
	{
		targetFlee = GameManager::FindNearestGameObjectWithTag(this, GameObject::MONSTER);

		if (targetFlee)
		{
			if ((targetFlee->getAbsolutePosition() - currentPosition).getLength() < fleeingDetectionRange)
				fleeingTimer = fleeingTime;

			if (fleeingTimer > 0.0f)
				state = FLEEING;
		}
	}
}

void Creature::ExecuteState()
{
	// Decide what to do depending on our state
	switch (state)
	{
		// Idling
		case IDLE:
		{
			moveSpeed = idleSpeed;

			// Get a new idle position on timer trigger
			if (idlePositionTimer <= 0.0f)
			{
				targetPosition = vector3df(rand() % (int)idlingRange * 2.0f - (int)idlingRange, 0.0f, rand() % (int)idlingRange * 2.0f - (int)idlingRange);
				idlePositionTimer = idlePositionTime;
			}

			break;
		}

		// Idling
		case FLEEING:
		{
			moveSpeed = fleeSpeed;

			// Set a target position away from the closest predator
			if (targetFlee)
				targetPosition = currentPosition + (currentPosition - targetFlee->getAbsolutePosition()).normalize() * moveSpeed;

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
			if (canAttack)
				agility += (canSeeTarget ? (-moveDirectionTarget.getLength() + chaseDetectionRange) / chaseDetectionRange * agilityFactor : 0.0);

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

	// Update states and movement
	Creature::UpdateState();
	Creature::ExecuteState();
	Creature::Move();
}