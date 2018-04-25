#include "PhysicsObject.h"
#include "GameManager.h"


#pragma region Namespaces
// Main namespace
using namespace irr;

// Namespaces of Irrlicht
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

PhysicsObject::PhysicsObject(ISceneNode* parentPar, ISceneManager* mgr, s32 id, const vector3df* startPosition, float massPar)
	: ISceneNode(parentPar, mgr, id)
{
	id_ = id;
	mass = massPar;
	velocity_ = vector3df(0);
	force_ = vector3df(0);
	position_ = *startPosition;

	parent = parentPar;

	gravityConstant = -9.8;
	buoyancyConstant = 9.8;
}


PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void PhysicsObject::render()
{

}

const aabbox3d<f32>& PhysicsObject::getBoundingBox() const
{
	return Box;
}

u32 PhysicsObject::getMaterialCount() const
{
	return 1;
}

SMaterial& PhysicsObject::getMaterial(u32 i)
{
	return Material;
}

void PhysicsObject::Update()
{
	if (tag != GameObject::GROUND && tag != GameObject::PLAYER) updatePosition();

	if (mesh)
	{
		mesh->setPosition(getAbsolutePosition());
		mesh->setRotation(getRotation());
	}
}

void PhysicsObject::updatePosition()
{
	position_ = getPosition();

	force_ += gravityForce() + dragForce() + buoyancyForce();
	verlet();
	resolveGround();

	setPosition(position_);
	
	force_ = vector3df(0);
	velocity_ *= 0.1;		// SHOULDN'T NEED THIS
}

void PhysicsObject::verlet()
{
	float timeStep = GameManager::deltaTimeMS;

	// VELOCITY VERLET
	position_ += velocity_ * timeStep + (0.5 * acceleration_ * timeStep * timeStep);
	vector3df newAcceleration = force_ / mass;
	vector3df avgAcceleration = (acceleration_ + newAcceleration) / 2;
	velocity_ += avgAcceleration * timeStep;
	acceleration_ = newAcceleration;
}

void PhysicsObject::resolveGround()
{
	float height;
	switch (tag)
	{
	case GameObject::MONSTER:
		height = 5000;
		break;
	case GameObject::CHEST:
	case GameObject::KEY:
		height = 0;
		break;
	default:
		height = 50;
		break;
	}
	if (position_.Y < -100 + height) {
		position_.Y = getAbsolutePosition().Y;
	}
}

vector3df PhysicsObject::dragForce()
{
	// 0.5 * densityWater * velocity^2 * dragCoefficient * crossSectionalArea
	return -0.5 * velocity_ * velocity_;
}

vector3df PhysicsObject::gravityForce()
{
	float gravity = gravityConstant * mass;
	return vector3df(0, gravity, 0);
}

vector3df PhysicsObject::buoyancyForce()
{
	// Only if not on the floor
	// densityWater * volumeObject
	// since denisity of human is around 1, take mass for volume
	// multiplied to balance with gravity
	//float volume = mesh->getTransformedBoundingBox().getVolume();
	float buoyancy = mass * buoyancyConstant;
	return vector3df(0, buoyancy, 0);
}


void PhysicsObject::turnToDirection(vector3df direction)
{
	setRotation(direction.getHorizontalAngle());
	if (mesh)
		mesh->setRotation(direction.getHorizontalAngle());
}

void PhysicsObject::addForce(vector3df force)
{
	force_ += force;
}

vector3df PhysicsObject::getVelocity()
{
	return velocity_;
}

void PhysicsObject::setVelocity(vector3df velocity)
{
	velocity_ = velocity;
}

// TODO: better drag: get something for those constants
// TODO: better buoyance: actually use different volumes somehow
