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
	updatePosition();

	if (mesh)
	{
		mesh->setPosition(getAbsolutePosition());
		mesh->setRotation(getRotation());
	}
}

void PhysicsObject::updatePosition()
{
	if (id_ != 9000)
	{
		position_ = getPosition();
	//} else
	//{
	//	position_ = parent->getPosition();
	//}

	force_ += gravityForce() + dragForce() + buoyancyForce();
	verlet();

	//if (id_ != 9000)
	//{
		setPosition(position_);
	}
	//else
	//{
	//	parent->setPosition(position_);
	//}

	force_ = vector3df(0);
	if (position_.Y < -85 && velocity_.Y < 0)
	{
		velocity_.Y = 1;
	}
	velocity_ *= 0.1;		// SHOULDN'T NEED THIS
}

void PhysicsObject::turnToDirection(vector3df direction)
{
	setRotation(direction.getHorizontalAngle());
	if (mesh)
		mesh->setRotation(direction.getHorizontalAngle());
}

vector3df PhysicsObject::dragForce()
{
	// 0.5 * densityWater * velocity^2 * dragCoefficient * crossSectionalArea
	return -0.5 * velocity_ * velocity_;
}

vector3df PhysicsObject::gravityForce()
{
	float gravity = (position_.Y < -85) ? 0 : gravityConstant * mass;
	return vector3df(0, gravity, 0);
}

vector3df PhysicsObject::buoyancyForce()
{
	// Only if not on the floor
	// densityWater * volumeObject
	// since denisity of human is around 1, take mass for volume
	// multiplied to balance with gravity
	float buoyancy = (position_.Y < -85) ? 0 : mass * buoyancyConstant;
	return vector3df(0, buoyancy, 0);
}

void PhysicsObject::verlet()
{
	float timeStep = GameManager::deltaTimeMS;

	// VELOCITY VERLET
	position_ += velocity_ * timeStep + (0.5 * acceleration_ * timeStep * timeStep);
	vector3df new_acceleration = force_ / mass;
	vector3df avg_acceleration = (acceleration_ + new_acceleration) / 2;
	velocity_ += avg_acceleration * timeStep;
	acceleration_ = new_acceleration;
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

void PhysicsObject::resolveGround()
{
	if (velocity_.Y < 0)
	{
		velocity_.Y = 0;
	}
	if (force_.Y < 0)
	{
		force_.Y = 0;
	}
	buoyancyConstant = 0;
}

// TODO: better drag: get something for those constants
// TODO: better buoyance: actually use different volumes somehow
