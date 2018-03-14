#include "Player.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Player::Player(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id)
{
	smgr = mgr;

	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices[0] = S3DVertex(-10, -20, 50, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
	Vertices[1] = S3DVertex(-5, -20, 10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
	Vertices[2] = S3DVertex(-10, -25, 40, 1, 1, 1, SColor(255, 255, 255, 0), 1, 0);
	Vertices[3] = S3DVertex(-15, -20, 10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);

	Vertices[4] = S3DVertex(10, -20, 50, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
	Vertices[5] = S3DVertex(15, -20, 10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
	Vertices[6] = S3DVertex(10, -25, 40, 1, 1, 1, SColor(255, 255, 255, 0), 1, 0);
	Vertices[7] = S3DVertex(5, -20, 10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);

	Box.reset(Vertices[0].Pos);
	for (s32 i = 1; i<8; ++i)
		Box.addInternalPoint(Vertices[i].Pos);
}


Player::~Player()
{
}

void Player::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void Player::render() 
{
	u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1, 4,6,7, 6,5,7, 5,4,7, 6,4,5 };

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 8, &indices[0], 8, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}

const aabbox3d<f32>&  Player::getBoundingBox() const
{
	return Box;
}

u32 Player::getMaterialCount() const
{
	return 1;
}

SMaterial& Player::getMaterial(u32 i)
{
	return Material;
}
