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

	// TODO: player movement not through camera
	// TODO: proper movement - on user input add to force
	// TODO: make it look like actual swimming with the back and forth thing

	updateAbsolutePosition();

	//UpdatePos();

	//vector3df currentPos = getAbsolutePosition();
	//setVelocity(currentPos - latestPos);
	//latestPos = currentPos;

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

}
