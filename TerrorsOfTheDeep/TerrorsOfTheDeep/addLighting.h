#ifndef addLighting_H
#define addLighting_H
#pragma once
#include <irrlicht.h>

// Class responsible for lighting.
class addLighting
{
	irr::scene::ISceneManager* smgr;
public:
	addLighting(irr::scene::ISceneManager* tempSmgr);

	void setSceneLight(float r, float g, float b, float a);
	irr::scene::ILightSceneNode* createPointLight(float r, float g, float b, float a, irr::core::vector3df pos, bool castShadow =false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* createDirectionalLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* createSpotLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
};

#endif