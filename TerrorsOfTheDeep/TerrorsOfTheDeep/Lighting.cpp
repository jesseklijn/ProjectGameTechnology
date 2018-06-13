#include "Lighting.h"

using namespace irr;
using namespace scene;

//! Add lights to a scene using these methods
/** Example use:
 * For ambient scene light:
 * Lighting(smgr).SetSceneLight(irr::video::SColorf(red,green,blue,alpha)
 *
 * For all Create[Name]Light methods:
 * Lighting(smgr).Create[Name]Light(SColorf(red, green, blue, alpha), vector3df(x,y,z), vector3df(x,y,z), float 1000-5000, bool, ISceneNode)
 * All CreateLight methods have two extra arguments after color and position, namely castShadow and parentNode.
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


//! Constructor
Lighting::Lighting(ISceneManager* tempSmgr)
{
	smgr = tempSmgr;
}

//! Sets the ambient scene light
/** \param color The color and transparency of the light. Requires values between 0 and 1
 */
void Lighting::SetSceneLight(irr::video::SColorf color)
{
	smgr->setAmbientLight(color);
}

//! Creates a point light e.g. a torch
/** \return An ILightSceneNode with pointlight behaviour 
 * \param color The color and transparency of the light. Requires values between 0 and 1
 * \param pos The position of the light
 * \param radius The radius of the light's influence. Correlates directly to intensity. Values should range between 1000 and 5000 for visible difference.
 * \param castShadow Turn pencil shadows on or off. Has a large impact on performance and is set to false by default.
 * \param parent Assign an ISceneNode as a parent of this light.
 */
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

//! Creates a directional light e.g. the sun.
/** \return An ILightSceneNode with directional light behaviour 
 * \param color The color and transparency of the light. Requires values between 0 and 1.
 * \param pos The position of the light.
 * \param dir The rotation of the light.
 * \param radius The radius of the light's influence. Correlates directly to intensity. Values should range between 1000 and 5000 for visible difference.
 * \param castShadow Turn pencil shadows on or off. Has a large impact on performance and is set to false by default.
 * \param parent Assign an ISceneNode as a parent of this light.
 */
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

//! Creates a spot light e.g. a flashlight
/** \return An ILightSceneNode with spotlight behaviour. 
 * \param color The color and transparency of the light. Requires values between 0 and 1.
 * \param pos The position of the light.
 * \param dir The rotation of the light.
 * \param radius The radius of the light's influence. Correlates directly to intensity. Values should range between 1000 and 5000 for visible difference.
 * \param castShadow Turn pencil shadows on or off. Has a large impact on performance and is set to false by default.
 * \param parent Assign an ISceneNode as a parent of this light.
 */
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