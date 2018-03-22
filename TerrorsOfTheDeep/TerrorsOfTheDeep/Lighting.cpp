#include "Lighting.h"

using namespace irr;
using namespace scene;

/*Example use:
For ambient scene light:
Lighting(smgr).SetSceneLight(irr::video::SColorf(r,g,b,a));

All CreateLight functions have two extra arguments after color and position, namely castShadow and parentNode. 
These default to false and 0.

ILightSceneNode* light1 = Lighting(smgr).CreatePointLight(irr::video::SColorf(r,g,b,a), irr::core::vector3df(0, 30, -40));
or
ILightSceneNode* light1 = Lighting(smgr).CreatePointLight(irr::video::SColorf(r,g,b,a), core::vector3df(0, 30, -40), true, 5);

Directional and Spotlight also require a vector3 dir for rotation
*/


// Constructor
Lighting::Lighting(ISceneManager* tempSmgr)
{
	smgr = tempSmgr;
}

// Updates the ambient scene light
void Lighting::SetSceneLight(irr::video::SColorf color)
{
	smgr->setAmbientLight(color);
}

// Creates a point light e.g. a torch
ILightSceneNode* Lighting::CreatePointLight(irr::video::SColorf color, irr::core::vector3df pos, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, color);
	light->setLightType(video::ELT_POINT);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

// Creates a directional light e.g. the sun
ILightSceneNode* Lighting::CreateDirectionalLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, color);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->setRadius(radius);
	light->setRotation(dir);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

// Creates a spot light e.g. a flashlight
ILightSceneNode* Lighting::CreateSpotLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow, ISceneNode* parentNode)
{
	
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, color);
	
	light->setLightType(video::ELT_SPOT);
	light->setRadius(radius);
	light->setRotation(dir);
	irr::video::SLight lightData;
	lightData = light->getLightData();
	lightData.Attenuation.X = 1.f;
	lightData.InnerCone = 90.f;
	lightData.OuterCone = 180.f;
	light->setLightData(lightData);
	
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}