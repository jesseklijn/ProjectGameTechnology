#pragma once
#include <ISceneUserDataSerializer.h>
#include <IShaderConstantSetCallBack.h>
#include <ILightSceneNode.h>
#include "GameManager.h"


/** Inherits from IShaderConstantSetCallBack
 *	This class is used to pass data from the program to shaders using the OnSetConstants method
 */

class CustomShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:
	CustomShaderCallBack();
	~CustomShaderCallBack();

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);
};
