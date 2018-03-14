#include "addLighting.h"

using namespace irr;
using namespace scene;

//class Lighting 
//{
//	ISceneManager* smgr;
//public:
//	void setSceneLight(float r,float g, float b, float a);
//	ILightSceneNode* createPointLight(float r, float g, float b, float a, core::vector3df pos, bool castShadow, ISceneNode* parentID);
//	ILightSceneNode* createDirectionalLight(float r, float g, float b, float a, core::vector3df pos, float radius, bool castShadow, ISceneNode* parentID);
//	ILightSceneNode* createSpotLight(float r, float g, float b, float a, core::vector3df pos, float radius, bool castShadow, ISceneNode* parentID);
//
//	Lighting(ISceneManager* tempSmgr);
//};
//
addLighting::addLighting(ISceneManager* tempSmgr)
{
	smgr = tempSmgr;
}


void addLighting::setSceneLight(float r, float g, float b, float a)
{
	smgr->setAmbientLight(video::SColorf(r, g, b, a));
}

ILightSceneNode* addLighting::createPointLight(float r, float g, float b, float a, irr::core::vector3df pos, bool castShadow = false, ISceneNode* parentNode = 0)
{

	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_POINT);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

ILightSceneNode* addLighting::createDirectionalLight(float r, float g, float b, float a, irr::core::vector3df pos, float radius, bool castShadow = false, ISceneNode* parentNode = 0)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_DIRECTIONAL);
	light->setRadius(radius);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}

ILightSceneNode* addLighting::createSpotLight(float r, float g, float b, float a, irr::core::vector3df pos, float radius, bool castShadow = false, ISceneNode* parentNode = 0)
{
	ILightSceneNode* light = smgr->addLightSceneNode(parentNode, pos, video::SColorf(r, g, b, a));
	light->setLightType(video::ELT_SPOT);
	light->setRadius(radius);
	if (castShadow)
		light->enableCastShadow(true);
	return light;
}
