#pragma once
#include "Shark.h"
#include "GameManager.h"

// Constructor
Shark::Shark(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: Monster(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{
	
}

// Destructor
Shark::~Shark()
{

}

void Shark::UpdateState()
{
	/* Determine core state
	Set defaults and gather basic shark information*/
	stateUpdateTime = stateUpdateTimeDefault;
	statePrevious = state;
	state = IDLE;
	currentPosition = getPosition();
	canSeeTarget = false;
	targetDistanceLast = targetDistance;
	targetDistance = INFINITY;

	// Get our potential target and target information
	if (canAttack)
	{
		targetAttack = Monster::GetTarget(targetPriority, attackCooldownTimer <= 0.0f ? true : false, chaseDetectionRange, true);
		if (targetAttack)
		{
			canSeeTarget = true;			
			targetDistance = (targetAttack->getAbsolutePosition() - getAbsolutePosition()).getLength();
		}

		// If target can potentially be spotted
		if (targetDistance < chaseDetectionRange)
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
	}

	// State switch detector
	if (statePrevious != state)
	{
		OnStateSwitch();
		statePrevious = state;
	}
}

void Shark::ExecuteState()
{
	// Decide what to do depending on our state
	switch (state)
	{
		// Idling when no target 
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

		// Chase the player/target
		case CHASING:
		{
			moveSpeed = chaseSpeed;
			targetPosition = targetAttack->getAbsolutePosition();

			// Max out our seek timer until we leave our state, so it ticks down when our target is lost
			seekTimer = seekTime;

			break;
		}

		// Attacking when in range
		case ATTACKING:
		{
			moveSpeed = chaseSpeed;
			stateUpdateTime = GameManager::deltaTimeMS;

			if ((Player*)targetAttack && targetDistance < attackKillRange)
				GameManager::gameOver = true;

			// When the shark at any time moves away from the target, we count this as an attack try.
			if (targetDistanceLast < targetDistance || targetDistance <= GameManager::Max(attackKillRange, moveSpeed))
			{
				attackCooldownTimer = attackCooldownTimeMin + rand() % (int)(attackCooldownTimeMax - attackCooldownTimeMin);

				/* Move away from the currently bitten target.
				For now it simply picks the furthest target until the attack cooldown finishes.*/
				targetPriority = FURTHEST;
			}

			break;
		}

		// Seeking for a short while if lost target
		case SEEKING:
		{
			moveSpeed = chaseSpeed;

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
			moveSpeed = idleSpeed;
			std::cout << "[ERROR] Invalid state, defaulting to IDLE..." << std::endl;
			break;
		}
	}
}

void Shark::OnStateSwitch()
{
	//std::cout << stateNames[(int)state] << std::endl;
}

void Shark::Update()
{
	// Update base
	Monster::Update();

	stateUpdateTimer = GameManager::Clamp(stateUpdateTimer - GameManager::deltaTimeMS, 0.0f, stateUpdateTime);
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	seekTimer = GameManager::Clamp(seekTimer - GameManager::deltaTimeMS, 0.0f, seekTime);
	attackCooldownTimer = GameManager::Clamp(attackCooldownTimer - GameManager::deltaTimeMS, 0.0f, attackCooldownTimeMax);
	if (stateUpdateTimer <= 0.0)
	{
		Shark::UpdateState();
		stateUpdateTimer = rand() % (int)stateUpdateTime;
	}
	Shark::ExecuteState();
	Shark::Move();
}