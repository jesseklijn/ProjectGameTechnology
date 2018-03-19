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

	void SetSceneLight(float r, float g, float b, float a);
	irr::scene::ILightSceneNode* CreatePointLight(float r, float g, float b, float a, irr::core::vector3df pos, bool castShadow =false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateDirectionalLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateSpotLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
};

#endif