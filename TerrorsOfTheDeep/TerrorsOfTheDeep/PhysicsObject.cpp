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

/**
 * A PhysicsObject is an ISceneNode that is effected by physics in its movement
 * 
 * GameObject : PhysicsObject : ISceneNode
 */
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
	dragCoefficient = 1;
	crossSectionalArea = 1;
	volume = mass;
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
	if (!isKinematic)
		UpdatePosition();
	
	/*if (mesh)
	{
		mesh->setPosition(getAbsolutePosition());
		mesh->setRotation(getRotation());
	}*/
}

/**
 * Updates the object's position every frame
 */
void PhysicsObject::UpdatePosition()
{
	position_ = getPosition();

	force_ += GravityForce() + DragForce() + BuoyancyForce();
	Verlet();

	// ResolveGround gets the monster stuck in the ground with mayor fps drop
	if (tag != GameObject::MONSTER)
		ResolveGround();

	setPosition(position_);
	
	force_ = vector3df(0);
	velocity_ *= 0.1;		// SHOULDN'T NEED THIS
}

/**
 * Uses Velocity Verlet integration to adjust the object's position
 * In the process calculates acceleration and updates current acceleration and velocity
 */
void PhysicsObject::Verlet()
{
	// timeStep is used to make movement framerate independant
	float timeStep = GameManager::deltaTimeMS;

	// Second order integration to calculate the new position
	position_ += velocity_ * timeStep + (0.5 * acceleration_ * timeStep * timeStep);

	// Newton's law to calculate acceleration 
	vector3df newAcceleration = force_ / mass;

	// Uses average of last and current acceleration to get new velosity
	vector3df avgAcceleration = (acceleration_ + newAcceleration) / 2;
	velocity_ += avgAcceleration * timeStep;
	acceleration_ = newAcceleration;
}

/**
 * ResolveGround prevents objects from moving under the ground
 * 
 * TODO: do this on collision with ground instead of here
 */
void PhysicsObject::ResolveGround()
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

/**
 * Calculates the current drag force working on the object
 * 
 * Depends on the density of water, current velocity, drag coëfficient, and corss sectional area
 * - because drag is always in the opposite direction from velocity
 */
vector3df PhysicsObject::DragForce()
{
	return -0.5 * velocity_ * velocity_ * dragCoefficient * crossSectionalArea;
}

/**
 * Calculates the current gravity force working on the object
 * Depends on the gravity constant and the object's mass
 */
vector3df PhysicsObject::GravityForce() 
{
	float gravity = gravityConstant * mass;
	return vector3df(0, gravity, 0);
}

//float volume = mesh->getTransformedBoundingBox().getVolume();

/**
 * Calculates the current buoyancy force working on the object
 * Depends on the density of water and the volume of the object
 */
vector3df PhysicsObject::BuoyancyForce()
{
	// density of water = 1
	float buoyancy = volume * buoyancyConstant;
	return vector3df(0, buoyancy, 0);
}

/**
 * Rotates the object in the given direction
 * 
 * Called from the Move method in GameObject
 */
void PhysicsObject::TurnToDirection(vector3df direction)
{
	setRotation(direction.getHorizontalAngle());
	if (mesh)
		mesh->setRotation(direction.getHorizontalAngle());
}

/**
 * Adds a vector to the current force working on the object
 * 
 * Called from the Move method in GameObject
 */
void PhysicsObject::AddForce(vector3df force)
{
	force_ += force;
}

vector3df PhysicsObject::GetVelocity()
{
	return velocity_;
}

void PhysicsObject::SetVelocity(vector3df velocity)
{
	velocity_ = velocity;
}

// TODO: better drag: get something for those constants
// TODO: better buoyance: actually use different volumes somehow
