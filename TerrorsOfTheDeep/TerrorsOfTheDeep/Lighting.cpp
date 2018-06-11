#include "Lighting.h"

using namespace irr;
using namespace scene;

/** Example use:
 * For ambient scene light:
 * Lighting(smgr).SetSceneLight(irr::video::SColorf(red,green,blue,alpha)
 *
 * For all Create[Name]Light functions:
 * Lighting(smgr).Create[Name]Light(SColorf(red, green, blue, alpha), vector3df(x,y,z), vector3df(x,y,z), float 1000-5000, bool, ISceneNode)
 * All CreateLight functions have two extra arguments after color and position, namely castShadow and parentNode.
 * These default to false and 0 respectively.
 *
 * ILightSceneNode* light1 = Lighting(smgr).CreatePointLight(irr::video::SColorf(r,g,b,a), irr::core::vector3df(0, 30, -40), irr::core::vector3df(0,90,0));
 * or
 * ILightSceneNode* light1 = Lighting(smgr).CreatePointLight(irr::video::SColorf(r,g,b,a), core::vector3df(0, 30, -40), irr::core::vector3df(0,90,0), true, 5);
 *
 * Note: Directional and Spotlight require a vector3df for rotation. PointLight, however, does not.
 *
 * Variable usage: 
 * SColorf expects floats between 0 and 1
 * Radius correlates to the intensity of the light and needs to be a large value, preferably between 1000 and 5000.
 * vector3df for rotation is used as degrees, not radians. If you want to rotate an object by 90 degrees along the x axis,
 * you'd use vector3df(90,0,0)
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
ILightSceneNode* Lighting::CreatePointLight(irr::video::SColorf color, irr::core::vector3df pos, float radius, bool castShadow, ISceneNode* parentNode)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, color);
	light->setLightType(video::ELT_POINT);
	light->setRadius((radius));
	//irr::video::SLight lightData;
	//light->setLightData(lightData);
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
	//irr::video::SLight lightData;
	//light->setLightData(lightData);
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
	irr::video::SLight lightData = light->getLightData();
	lightData.Attenuation.X = 0.75f;
	lightData.InnerCone = 20.f;
	lightData.OuterCone = 75.f;
	lightData.Falloff = 20.f;
	lightData.SpecularColor = video::SColorf(0.0f, 0.0f, 0.0f, 0.f);
	light->setLightData(lightData);

	if (castShadow)
		light->enableCastShadow(true);
	return light;
}