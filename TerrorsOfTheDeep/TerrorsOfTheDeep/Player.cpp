#include "Player.h"
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
	GameObject::setTag(GameObject::PLAYER);
	//buoyancyConstant = 9.79;

	irrDevice = GameManager::device;
	smgr = mgr;

	latestPos = getAbsolutePosition();
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

	// TODO: player movement not through camera
	// TODO: proper movement - on user input add to force
	// TODO: make it look like actual swimming with the back and forth thing

	updateAbsolutePosition();

	UpdatePos();

	//vector3df currentPos = getAbsolutePosition();
	//setVelocity(currentPos - latestPos);
	//latestPos = currentPos;
}

void Player::render()
{
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

void Player::UpdatePos()
{
	if (mesh != nullptr)
	{
		vector3df rot = smgr->getActiveCamera()->getRotation();
		mesh->setRotation(rot);
	}
}
