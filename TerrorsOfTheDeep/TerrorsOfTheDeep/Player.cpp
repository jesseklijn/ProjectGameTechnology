#pragma once
#include "Player.h"
#pragma once
#include "GameManager.h"

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion


// Constructor
Player::Player(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{
	irrDevice = GameManager::device;
	smgr = mgr;

	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices[0] = S3DVertex(-10, -20, 50, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
	Vertices[1] = S3DVertex(-5, -20, 10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
	Vertices[2] = S3DVertex(-10, -25, 40, 1, 1, 1, SColor(255, 255, 255, 0), 1, 0);
	Vertices[3] = S3DVertex(-15, -20, 10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);

	Vertices[4] = S3DVertex(10, -20, 50, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
	Vertices[5] = S3DVertex(5, -20, 10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
	Vertices[6] = S3DVertex(10, -25, 40, 1, 1, 1, SColor(255, 255, 255, 0), 1, 0);
	Vertices[7] = S3DVertex(15, -20, 10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);

	Box.reset(Vertices[0].Pos);
	for (s32 i = 1; i<8; ++i)
		Box.addInternalPoint(Vertices[i].Pos);

	then = irrDevice->getTimer()->getTime();
	deltaX = 0.05;
	deltaZ = 0.05;
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

void Player::Update()
{
	// Inherit base class Update
	GameObject::Update();

	updatePos();
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

void Player::updatePos()
{
	// if W down, move both hands
	// if moving mouse in direction, move other hands

	u32 difference = Vertices[5].Pos.X - Vertices[1].Pos.X;
	if (difference > 60 || difference < 2) {
		deltaX *= -1;
		deltaZ *= -1;
	}

	for (int i = 0; i < 4; i++) {
		Vertices[i].Pos.X -= deltaX;
		Vertices[i].Pos.Z -= deltaZ;
	}
	for (int i = 4; i < 8; i++) {
		Vertices[i].Pos.X += deltaX;
		Vertices[i].Pos.Z -= deltaZ;
	}
}

// In main:
// Player player = Player(smgr->getActiveCamera(), smgr, -1111, device);
// In while:
// player.updatePos();