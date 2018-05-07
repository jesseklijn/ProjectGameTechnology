#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Creature.h"
#include <vector>

// Defines a creature monster, as in a boss enemy.
class Monster : public Creature
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
	enum TargetPriority
	{
		CLOSEST,
		FURTHEST
	};
	TargetPriority targetPriority = CLOSEST;

	std::vector<GameObject*> attackedTargets;
	float detectionRangeOverrideDist = 6500.0f;


	// Functions
	virtual void Update();
	virtual GameObject* PlayerCanBeSeen(double detectionRange = INFINITY, bool visibilityCheck = false);
	virtual GameObject* GetTarget(TargetPriority priorityMode = CLOSEST, bool playerDetectOverride = true, double detectionRange = INFINITY, bool visibilityCheck = false);
	virtual bool IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition);
	virtual bool IsInSight(GameObject* targetObject);

private:
	std::vector<GameObject*> tempTargets;
};