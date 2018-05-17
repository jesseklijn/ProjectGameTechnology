#pragma once
#include <irrlicht.h>
#include "DynamicUpdater.h"

class PhysicsObject : public irr::scene::ISceneNode, public DynamicUpdater
{
	//float density_;
	irr::core::vector3df position_;
	irr::core::vector3df velocity_;
	irr::core::vector3df acceleration_;
	irr::core::vector3df force_;
	irr::s32 id_;

	void UpdatePosition();

	irr::core::vector3df DragForce();
	irr::core::vector3df GravityForce();
	irr::core::vector3df BuoyancyForce();
	void Verlet();
	void ResolveGround();

public:
	float gravityConstant;
	float buoyancyConstant;
	float mass;
	float volume;
	float dragCoefficient;
	float crossSectionalArea;
	irr::scene::ISceneNode* parent;

	PhysicsObject(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		const irr::core::vector3df* startPosition, float mass = 1);
	~PhysicsObject();

	// Variables
	bool isKinematic = false;
	irr::scene::IAnimatedMeshSceneNode* mesh;
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::SMaterial Material;
	irr::scene::ITriangleSelector* selector;
	Tag tag = NONE;

	// Functions
	void render() override;
	void OnRegisterSceneNode() override;
	const irr::core::aabbox3d<irr::f32>& getBoundingBox() const override;
	irr::u32 getMaterialCount() const override;
	irr::video::SMaterial& getMaterial(irr::u32 i) override;

	virtual void Update();

	void AddForce(irr::core::vector3df force);
	void TurnToDirection(irr::core::vector3df direction);
	irr::core::vector3df GetVelocity();
	void SetVelocity(irr::core::vector3df velocity);
};
