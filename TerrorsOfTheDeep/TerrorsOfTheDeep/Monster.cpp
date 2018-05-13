#pragma once
#include "Monster.h"
#include "GameManager.h"

// Constructor
Monster::Monster(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, 
	bool detectCollision, bool castsShadows)
	: Creature(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision, castsShadows)
{
	GameObject::setTag(GameObject::MONSTER);

	canAttack = true;
	canFlee = false;

	rotationLerp = 0.000035;
	idleSpeed = 325.0f;
	moveSpeed = idleSpeed;	
	chaseSpeed = idleSpeed * chaseSpeedMultiplier;
	attackRange = 1250.0f;
	attackKillRange = chaseSpeed;

	targetTags.push_back(GameObject::PLAYER);
	targetTags.push_back(GameObject::CREATURE);
}

// Destructor
Monster::~Monster()
{

}

void Monster::UpdateState()
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

void Monster::ExecuteState()
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

		if ((Player*)targetAttack && targetDistance <= attackKillRange)
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

/* Finds the player in-game and decides whether or not it is a valid target.
Detection range and a raycast can be enabled for specified checks.

This is a shortcut instead of having to specify extra Player override functionality
in the other GameManager::FindX(); functions, which should be for general usage. */
GameObject* Monster::PlayerCanBeSeen(double detectionRange, bool visibilityCheck)
{
	Player* player = (Player*)GameManager::FindObjectWithTag<GameObject>(PLAYER, GameManager::gameObjects);
	if (player)
	{
		float currentDistance = (player->getAbsolutePosition() - getAbsolutePosition()).getLength();

		// If the monster is very far away, ignore raycasting and make it follow player regardless
		if (currentDistance >= detectionRangeOverrideDist)
			return player;
		else if (currentDistance < detectionRange)
		{
			if (!visibilityCheck || Monster::IsInSight(getAbsolutePosition(), player->getAbsolutePosition()))
			{
				return player;
			}
		}
	}
	return nullptr;
}

/* Returns a target based on this monster's tags. Optionally a player detection override, detection range and visibility check can
be enabled for more specific searches. Returns a nullptr if no target was found with the given specifications. */
GameObject* Monster::GetTarget(Monster::TargetPriority priorityMode, bool playerDetectOverride, double detectionRange, bool visibilityCheck)
{
	// If the player should override all other detection
	if (playerDetectOverride)
	{
		// Return the player if he/she's a valid target
		GameObject* player = (Player*)Monster::PlayerCanBeSeen(detectionRange, visibilityCheck);
		if (player != nullptr)
			return player;
	}

	switch (priorityMode)
	{		
		case CLOSEST:
		{
			// Finds the nearest GameObject that satisfies this Monster's prey tag list, regardless of class.
			return GameManager::gameObjects[rand() % (GameManager::gameObjects.size() - 1)];//GameManager::FindNearestGameObjectWithTags<GameObject>(this, targetTags, GameManager::gameObjects, detectionRange, visibilityCheck);
			break;
		}

		case FURTHEST:
		{
			// Finds the furthest GameObject that satisfies this Monster's prey tag list, regardless of class.
			return GameManager::gameObjects[rand() % (GameManager::gameObjects.size() - 1)]; //GameManager::FindFurthestGameObjectWithTags<GameObject>(this, targetTags, GameManager::gameObjects, detectionRange, visibilityCheck);
			break;
		}

		default:
		{
			std::cout << "[ERROR] Invalid priority mode, returning nullptr...";
			return nullptr;
		}
	}
}

// Overridden (from GameObject) Update-loop
void Monster::Update()
{
	/* Run the Update() of our base class.
	Since this is a boss type creature, we have our own Update() in children, so don't inherit Update() from Creature.*/
	GameObject::Update();

	stateUpdateTimer = GameManager::Clamp(stateUpdateTimer - GameManager::deltaTimeMS, 0.0f, stateUpdateTime);
	idlePositionTimer = GameManager::Clamp(idlePositionTimer - GameManager::deltaTimeMS, 0.0f, idlePositionTime);
	seekTimer = GameManager::Clamp(seekTimer - GameManager::deltaTimeMS, 0.0f, seekTime);
	attackCooldownTimer = GameManager::Clamp(attackCooldownTimer - GameManager::deltaTimeMS, 0.0f, attackCooldownTimeMax);
	if (stateUpdateTimer <= 0.0)
	{
		UpdateState();
		stateUpdateTimer = rand() % (int)stateUpdateTime;
	}
	ExecuteState();
	Move();
}

// Checks whether a target position can be seen.
bool Monster::IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition)
{
	ISceneNode* target = GameManager::PerformRaycast(startPosition, endPosition);
	
	// If I didn't find anything in the way
	if (!target)
		return true;
	else
		return false;
}

// Checks whether a player can be seen.
bool Monster::IsInSight(GameObject* targetObject)
{
	GameObject &tar = *targetObject;

	// If this GameObject has no mesh, return false
	if (!tar.mesh)
		return false;

	// If I hit a triangle of the target
	ISceneNode* target = GameManager::PerformRaycast(getPosition(), tar.getPosition());
	if (target)
		return true;
	else
		return false;
}