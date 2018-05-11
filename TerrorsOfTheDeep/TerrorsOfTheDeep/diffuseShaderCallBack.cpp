#include <IShaderConstantSetCallBack.h>
#include <ISceneNode.h>
#include <IMaterialRendererServices.h>
#include "diffuseShaderCallBack.h"
#include "GameManager.h"
#include <irrMath.h>

DiffuseShaderCallBack::DiffuseShaderCallBack()
{
	
}

DiffuseShaderCallBack::~DiffuseShaderCallBack()
{
	
}

// Used for passing data to the shader, useful for positions, colours and heavier calculations
void DiffuseShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
	irr:core::matrix4 proj = GameManager::driver->getTransform(irr::video::ETS_PROJECTION);
	irr::core::matrix4 view = GameManager::driver->getTransform(irr::video::ETS_VIEW);
	irr::core::matrix4 world = GameManager::driver->getTransform(video::ETS_WORLD);

	irr::core::matrix4 WVP = proj * view * world;
	const float LIGHT_INTENSITY = 1.f;

	f32 lightPos[3] = { light->getAbsolutePosition().X, light->getAbsolutePosition().Y, light->getAbsolutePosition().Z };
	f32 lightIntensity[1] = { LIGHT_INTENSITY };
	f32 lightDistance[1] = { pow(node->getPosition().getDistanceFrom(light->getPosition()),2)};
	f32 ambientLightCol[4] = { GameManager::smgr->getAmbientLight().r, GameManager::smgr->getAmbientLight().g, GameManager::smgr->getAmbientLight().b, GameManager::smgr->getAmbientLight().a };
	f32 camPos[3] = { GameManager::smgr->getActiveCamera()->getAbsolutePosition().X, GameManager::smgr->getActiveCamera()->getAbsolutePosition().Y, GameManager::smgr->getActiveCamera()->getAbsolutePosition().Z };
	
	services->setVertexShaderConstant("world", world.pointer(), 16);
	services->setVertexShaderConstant("WVP", WVP.pointer(), 16);
	
	services->setVertexShaderConstant("lightPos", lightPos, 3);
	services->setVertexShaderConstant("lightDistance", lightDistance, 1);
	services->setVertexShaderConstant("camPos", camPos, 3);
	services->setVertexShaderConstant("lightCol", lightIntensity, 1);
	services->setVertexShaderConstant("ambientLightCol",ambientLightCol , 4);
	// Easy to copy and paste line incase more Shader Constants needs to be passed.
	// services->setVertexShaderConstant("", 0, 0);
}



