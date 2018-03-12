#pragma once

#include <irrlicht.h>
namespace irr
{
	class Monster
	{
	public:
		Monster();
		~Monster();

		scene::ISceneNode* PerformRaycast(core::vector3df startPosition, core::vector3df endPosition);
	};
}

