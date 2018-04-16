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

	//parent_ = parent;

	gravity = vector3df(0, -0.001, 0);
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
	if (id_ != 9000)
	{
		updatePosition();
	}

	if (mesh)
	{
		mesh->setPosition(getAbsolutePosition());
		mesh->setRotation(getRotation());
	}
}

void PhysicsObject::updatePosition()
{
	position_ = getPosition();

	if (velocity_.Y > -0.1)
	{
		force_ += gravity;
	}
	vector3df acceleration = (1 / mass_) * force_;
	velocity_ += acceleration;
	if (position_.Y > -50)
	{
		position_ += velocity_;
	}
	setPosition(position_);

	force_ = vector3df(0);
	velocity_ = vector3df(0);
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

// TODO: proper movement - on user input add to force
// TODO: proper gravity - cap on velocity
// TODO: friction!
// TODO: buoyance