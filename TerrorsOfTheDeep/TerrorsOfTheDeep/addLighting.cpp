#include "addLighting.h"

using namespace irr;
using namespace scene;

/*Example use:
For ambient scene light:
addLighting(smgr).setSceneLight(r, g, b, a);

All createLight functions have two extra arguments after colour and position, namely castShadow and parentNode. 
These default to false and 0.

ILightSceneNode* light1 = addLighting(smgr).createPointLight(r, g, b, a, core::vector3df(0, 30, -40));
or
ILightSceneNode* light1 = addLighting(smgr).createPointLight(r, g, b, a, core::vector3df(0, 30, -40), true, 5);

Directional and Spotlight also require a vector3 dir for rotation
*/


// Constructor
addLighting::addLighting(ISceneManager* tempSmgr)
{
	smgr = tempSmgr;
}

// Updates the ambient scene light
void addLighting::setSceneLight(float r, float g, float b, float a)
{
	smgr->setAmbientLight(video::SColorf(r, g, b, a));
}

// Creates a point light e.g. a torch
ILightSceneNode* addLighting::createPointLight(float r, float g, float b, float a, irr::core::vector3df pos, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_POINT);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

// Creates a directional light e.g. the sun
ILightSceneNode* addLighting::createDirectionalLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_DIRECTIONAL);
	light->setRadius(radius);
	light->setRotation(dir);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

// Creates a spot light e.g. a flashlight
ILightSceneNode* addLighting::createSpotLight(float r, float g, float b, float a, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_SPOT);
	light->setRadius(radius);
	light->setRotation(dir);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}