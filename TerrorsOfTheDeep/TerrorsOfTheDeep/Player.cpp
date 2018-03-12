#include "Player.h"

Player::Player(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id)
{
	smgr = mgr;

	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
	Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
	Vertices[2] = S3DVertex(0, 20, 0, 1, 1, 1, SColor(255, 255, 255, 0), 1, 0);
	Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);

	Box.reset(Vertices[0].Pos);
	for (s32 i = 1; i<4; ++i)
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
	u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
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
