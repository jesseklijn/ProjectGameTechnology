#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


class Player : public ISceneNode
{
public:
	ISceneManager* smgr;

	aabbox3d<f32> Box;
	S3DVertex Vertices[4];
	SMaterial Material;

	// int oxygen, stamina...
	Player(ISceneNode* parent, ISceneManager* mgr, s32 id);
	~Player();

	virtual void render();
	virtual void OnRegisterSceneNode();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
};
