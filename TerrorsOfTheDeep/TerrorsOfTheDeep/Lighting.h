#ifndef Lighting_H
#define Lighting_H
#pragma once
#include <irrlicht.h>

/** Class responsible for lighting.
 * Simplified ILightSceneNode creation to allow all the developers to add lights to the scene without need of prior knowledge of the Irrlicht ILightSceneNode and lightData.
 * Comes with predefined lightData settings to ensure homogenous light behaviour.
 *  
 * Requires an initialized ISceneManager, otherwise the lights can't be assigned to a Scene.
 * The parameters required for the Create[Name]Light functions are mostly the same, however the functions are still split up in order to increase code readability and usability.
 * The CreatePointLight function does not require the vector3df rotation.
 * The type of light is clearly defined in the name of the function.
 *  
 * SetSceneLight is used for ambient light, which only requires an SColorf type variable to set the colour.
 *  
 * SColorf color - this requires 4 floats that represent the red, green, blue and alpha data. This is used for the colour of the light. 
 * vector3df pos - this requires 4 floats that represent the x,y and z axis position. This is in local space, incase a parent is assigned it will be relative to the parent's position
 * vector3df dir - this requires 4 floats that represent rotation long the x, y and z axis. The rotation is in local space, incase a parent is assigned it will be relative to the parent's rotation.
 * float radius - A float that defines the light's radius of influence. This correlates directly to intensity.
 * bool castShadow - Allows the use of pencil shadows being cast from the light source. !!Resource Heavy!!
 * ISceneNode parent -  Assigns a parent ISceneNode to the created ILightSceneNode
*/
class Lighting
{
	irr::scene::ISceneManager* smgr;
public:
	Lighting(irr::scene::ISceneManager* tempSmgr);

	void SetSceneLight(irr::video::SColorf color);
	irr::scene::ILightSceneNode* CreatePointLight(irr::video::SColorf color, irr::core::vector3df pos, float radius, bool castShadow =false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateDirectionalLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
	irr::scene::ILightSceneNode* CreateSpotLight(irr::video::SColorf color, irr::core::vector3df pos, irr::core::vector3df dir, float radius, bool castShadow = false, irr::scene::ISceneNode* parentID = 0);
};

#endif