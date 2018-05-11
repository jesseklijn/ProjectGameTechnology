#include <ISceneUserDataSerializer.h>
#include <IShaderConstantSetCallBack.h>
#include <ILightSceneNode.h>

namespace irr {
	namespace video {
		class IMaterialRendererServices;
	}
}

class DiffuseShaderCallBack : public irr::video::IShaderConstantSetCallBack
{
public:
	DiffuseShaderCallBack();
	~DiffuseShaderCallBack();
	irr::scene::ILightSceneNode* light;
	irr::scene::ISceneNode* node;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) override;
};
