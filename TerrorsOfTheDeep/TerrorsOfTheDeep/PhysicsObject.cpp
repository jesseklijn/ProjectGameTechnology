#include "PhysicsObject.h"


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

PhysicsObject::PhysicsObject(ISceneNode* parent, ISceneManager* mgr, s32 id, const vector3df* startPosition, float mass)
	: ISceneNode(parent, mgr, id)
{
	id_ = id;
	mass_ = mass;
	velocity_ = vector3df(0);
	force_ = vector3df(0);
	position_ = *startPosition;

	parent = parent;

	gravityConstant = vector3df(0, -0.001, 0);
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
	} else
	{
		position_ = parent->getPosition();
	}

	if (velocity_.Y > -0.1)
	{
		force_ += gravityForce();
	}
	force_ += dragForce();
	force_ += buoyancyForce();

	// VELOCITY VERLET
	//position_ += velocity_ * time_step + (0.5 * acceleration_ * time_step ^ 2);
	//vector3df new_acceleration = force_ / mass_;
	//vector3df avg_acceleration = (acceleration_ + new_acceleration) / 2;
	//velocity_ += avg_acceleration * time_step;

	position_ += velocity_ + (0.5 * acceleration_);
	vector3df newAcceleration = force_ / mass_;
	vector3df avgAcceleration = (acceleration_ + newAcceleration) / 2;
	velocity_ += avgAcceleration;
	acceleration_ = newAcceleration;

	if (id_ != 9000)
	{
		setPosition(position_);
	}
	else
	{
		parent->setPosition(position_);
	}

	force_ = vector3df(0);
	if (position_.Y < -85)
	{
		velocity_.Y = 0;
	}
	velocity_ *= 0.1;
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
	return 0.5 * velocity_ * velocity_;
}

vector3df PhysicsObject::gravityForce()
{
	return gravityConstant * mass_;
}

vector3df PhysicsObject::buoyancyForce()
{
	// densityWater * volumeObject
}

void PhysicsObject::addForce(vector3df force)
{
	force_ += force;
}

// TODO: better draaaaag!
// TODO: better buoyance
// TODO: use timesteps
// TODO: player movement not through camera
// TODO: proper movement - on user input add to force
// TODO: proper gravity - cap on velocity
