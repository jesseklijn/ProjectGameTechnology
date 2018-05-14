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

	irr::core::matrix4 PVW = proj * view * world;
	irr::core::matrix4 transposedW = world.getTransposed();
	const float LIGHT_INTENSITY = 1.f;

	vector3df lightPos = GameManager::smgr->getActiveCamera()->getAbsolutePosition();
	irr::video::SColorf lightCol = irr::video::SColorf();
	f32 lightIntensity[1] = { LIGHT_INTENSITY };
	f32 lightDistance[1] = { pow(node->getPosition().getDistanceFrom(light->getPosition()),2)};
	f32 ambientLightCol[4] = { GameManager::smgr->getAmbientLight().r, GameManager::smgr->getAmbientLight().g, GameManager::smgr->getAmbientLight().b, GameManager::smgr->getAmbientLight().a };

	services->setVertexShaderConstant("world", world.pointer(), 16);
	services->setVertexShaderConstant("worldInverse", transposedW.pointer(), 16);
	services->setVertexShaderConstant("PVW", PVW.pointer(), 16);
	
	services->setVertexShaderConstant("lightPos", reinterpret_cast<f32*>(&lightPos), 3);
	services->setVertexShaderConstant("lightDistance", lightDistance, 1);
	services->setVertexShaderConstant("lightCol", lightIntensity, 1);
	services->setVertexShaderConstant("ambientLightCol",ambientLightCol , 4);
	/* Easy to copy and paste line incase more Shader Constants needs to be passed.
	/* Example:
	 * services->setVertexShaderConstant("vertexVarName", varName, int arrayIndexCount);
	 * 
	 * This expects a f32 array with specified index count, however another option is
	 *  to reinterpret_cast<f32*>(&ref) it. This still requires you to manually add an index count so keep that in mind.
	 * Example:
	 * services->setVertexShaderConstant("lightPos", reinterpret_cast<f32*>(&lightPos), 3);
	 */
}



