#ifndef Lighting_H
#define Lighting_H
#pragma once
#include <irrlicht.h>

// Class responsible for lighting.
class Lighting
{
	irr::scene::ISceneManager* smgr;
public:
	Lighting(irr::scene::ISceneManager* tempSmgr);

	void SetSceneLight(irr::video::SColorf color);
	irr::scene::ILightSceneNode* CreatePointLight(irr::video::SColorf color, irr::core::vector3df pos, bool castShadow =false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateDirectionalLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateSpotLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
};

#endif