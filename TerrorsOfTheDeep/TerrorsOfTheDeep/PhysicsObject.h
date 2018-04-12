#pragma once
#include <irrlicht.h>
class PhysicsObject : public irr::scene::ISceneNode
{
	irr::core::vector3df position_;
	irr::core::vector3df velocity_;
	irr::core::vector3df force_;
	irr::s32 id_;
	float mass_;

public:
	irr::core::vector3df gravity;
	irr::scene::ISceneNode* parent_;

	PhysicsObject(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		const irr::core::vector3df* startPosition, float mass = 1);
	~PhysicsObject();

	// Variables
	irr::scene::IAnimatedMeshSceneNode* mesh;
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::SMaterial Material;
	irr::scene::ITriangleSelector* selector;

	// Functions
	void render() override;
	void OnRegisterSceneNode() override;
	const irr::core::aabbox3d<irr::f32>& getBoundingBox() const override;
	irr::u32 getMaterialCount() const override;
	irr::video::SMaterial& getMaterial(irr::u32 i) override;

	virtual void Update();
	void UpdatePosition();
};

