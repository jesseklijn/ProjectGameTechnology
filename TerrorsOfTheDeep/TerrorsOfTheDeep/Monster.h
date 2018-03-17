#pragma once
#include <irrlicht.h>

// Defines a monster, as in a boss enemy.
class Monster
{
public:
	// Constructor and deconstructor
	Monster();
	~Monster();

	// Functions
	bool IsInSight(irr::core::vector3df* startPosition, irr::core::vector3df* endPosition);
};