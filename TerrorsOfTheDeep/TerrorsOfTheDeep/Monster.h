#pragma once
#include "GameObject.h"
#pragma once
#include "Player.h"
#pragma once
#include "Creature.h"
#pragma once
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

	// Functions
	virtual void Update();
	virtual GameObject* PlayerCanBeSeen(float detectionRange = INFINITY, bool visibilityCheck = false);
	virtual GameObject* GetTarget(TargetPriority priorityMode = CLOSEST, bool playerDetectOverride = true, float detectionRange = INFINITY, bool visibilityCheck = false);
	virtual bool IsInSight(irr::core::vector3df startPosition, irr::core::vector3df endPosition);
	virtual bool IsInSight(GameObject* targetObject);

private:
	std::vector<GameObject*> tempTargets;
};