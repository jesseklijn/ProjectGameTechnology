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

	// Create bubble particle system and emitter
	particleSystem =
		smgr->addParticleSystemSceneNode(false);
	particleEmitter = particleSystem->createBoxEmitter(
		core::aabbox3d<f32>(-24, -50, -24, 24, -25, 24),	// Emitter bounding box
		core::vector3df(0.0f, 0.03f, 0.0f),					// Base particle direction
		25, 35,												// Emission rate (min, max)
		video::SColor(0, 50, 50, 50),						// Darkest color particle
		video::SColor(0, 200, 200, 200),					// Brightest color particle
		2500, 4500,											// Lifetime (min, max)
		10.0f,												// Max angle deviation
		core::dimension2df(5.0f, 5.0f),						// Min size
		core::dimension2df(8.0f, 8.0f));					// Max size
	particleSystem->setEmitter(particleEmitter);

	// Make sure the particles fade away
	scene::IParticleAffector* particleFadeOut = particleSystem->createFadeOutParticleAffector();
	particleSystem->addAffector(particleFadeOut);
	particleFadeOut->drop();

	particleSystem->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	particleSystem->setMaterialFlag(video::EMF_LIGHTING, false);
	particleSystem->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	particleSystem->setMaterialTexture(0, GameManager::driver->getTexture("../media/Particles/bubble.png"));
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

	if (particleSystem)
	{
		particleSystem->setPosition(getAbsolutePosition());

		// Timer that triggers the start of a new burst of bubbles from the player
		bubbleStartTimer = GameManager::Clamp(bubbleStartTimer - GameManager::deltaTimeMS, 0.0f, bubbleStartTime);
		if (bubbleStartTimer == 0.0f)
		{
			particleSystem->setEmitter(particleEmitter);
			bubbleDurationTimer = bubbleDurationTime;

			// Set the timer again for the next bubble trigger
			bubbleStartTimer = bubbleStartTime;
		}

		// Timer that triggers when a burst is supposed to end
		bubbleDurationTimer = (bubbleDurationTimer > 0.0f) ? GameManager::Clamp(bubbleDurationTimer - GameManager::deltaTimeMS, 0.0f, bubbleDurationTime) : -1.0f;
		if (bubbleDurationTimer == 0.0f)
		{
			particleSystem->setEmitter(0);

			/* Disable this timer by setting it below 0.
			It should only run after the bubbleStartTimer goes off and not repeat itself automatically. */
			bubbleDurationTimer = -1.0f;
		}
	}
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

	// applies rotation to y-axis direction
	vector3df upDirection = r * vector3df(0, 1, 0);
	upDirection.normalize();

	vector3df movement = vector3df(0, 0, 0);

	if (canMove)
	{
		// Front
		if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_W))
			movement += frontDirection;

		// Left
		if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_A))
			movement -= sideDirection;

		// Back
		if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_S))
			movement -= frontDirection;

		// Right
		if (GameManager::eventManager.IsKeyDown(irr::KEY_KEY_D))
			movement += sideDirection;

		// Up
		if (GameManager::eventManager.IsKeyDown(irr::KEY_SPACE))
			movement += upDirection;

		// Down
		if (GameManager::eventManager.IsKeyDown(irr::KEY_LCONTROL))
			movement -= upDirection;

		MoveParent(movement);

		if (mesh != nullptr)
			mesh->setPosition(vector3df(0, 0, 0));
	}
}

/// Moves the player's parent (camera) according to the given movement vector.
void Player::MoveParent(vector3df parentMovement)
{
	getParent()->setPosition(getParent()->getPosition() + speed * parentMovement.normalize());
}

void Player::NotifyCollision(vector3df objPosition)
{
	isColliding = true;
	colPos = objPosition;
}
