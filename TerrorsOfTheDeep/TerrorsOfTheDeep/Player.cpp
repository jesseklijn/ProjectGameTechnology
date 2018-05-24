#include "Player.h"
#include "GameManager.h"
#include <matrix4.h>

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
	float movementSpeed,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{
	GameObject::setTag(GameObject::PLAYER);

	speed = movementSpeed;

	irrDevice = GameManager::device;
	smgr = mgr;
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

	updateAbsolutePosition();

	UpdatePos();

	isColliding = false;
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
	ICameraSceneNode* camera = smgr->getActiveCamera();

	// gets direction camera is facing
	vector3df frontDirection = camera->getTarget() - camera->getAbsolutePosition();
	frontDirection.normalize();

	// gets (non vertical) rotation of camera
	quaternion r = quaternion();
	r.rotationFromTo(vector3df(0, 0, 1), vector3df(frontDirection.X, 0, frontDirection.Z));

	// applies rotation to x-axis direction
	vector3df sideDirection = r * vector3df(1, 0, 0);
	sideDirection.normalize();

	// Front
	if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_W))
	{
		MoveParent(frontDirection);
	}
	// Left
	if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_A))
	{
		MoveParent(-sideDirection);
	}
	// Back
	if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_S))
	{
		MoveParent(-frontDirection);
	}
	// Right
	if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_D))
	{
		MoveParent(sideDirection);
	}
	// Up
	if (GameManager::eventManager.IsKeyDown(irr::KEY_SPACE))
	{
		MoveParent(vector3df(0, 1, 0));
	}
	// Down
	if (GameManager::eventManager.IsKeyDown(irr::KEY_LCONTROL))
	{
		MoveParent(vector3df(0, -1, 0));
	}

	if (mesh != nullptr)
	{
		mesh->setPosition(vector3df(0, 0, 0));
	}

}

void Player::MoveParent(vector3df movement)
{
	vector3df newPos = parent->getAbsolutePosition() + speed * movement;

	// prevents moving towards the object that the camera is colliding with
	bool collisionStop = isColliding && (colPos - newPos).getLength() < (colPos - parent->getAbsolutePosition()).getLength();
	bool groundStop = newPos.Y < 0 && movement.Y < 0;
	if (!collisionStop && !groundStop)
	{
		parent->setPosition(newPos);
	}
}

void Player::NotifyCollision(vector3df objPosition)
{
	isColliding = true;
	colPos = objPosition;
}
