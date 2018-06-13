#pragma once
#include "irrlicht.h"
#include "PhysicsObject.h"
#include "DynamicUpdater.h"

class GameObject : public PhysicsObject
{
public:
	// Constructor
	GameObject(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0,
		bool detectCollision = true, bool castsShadows = false, float mass = 1);

	// Destructor
	virtual ~GameObject();

	// Variables
	irr::scene::IAnimatedMeshSceneNode* mesh;
	irr::core::aabbox3d<irr::f32> box;
	irr::video::SMaterial material;
	irr::scene::ITriangleSelector* selector;
	Tag tag = NONE;

	bool canMove = true;
	float moveSpeed = 0.25f;
	bool isColliding = false;
	irr::core::vector3df colPos;

	// Functions
	void render() override;
	void OnRegisterSceneNode() override;
	const irr::core::aabbox3d<irr::f32>& getBoundingBox() const override;
	irr::u32 getMaterialCount() const override;
	irr::video::SMaterial& getMaterial(irr::u32 i) override;

	virtual void Update();
	virtual void FixedUpdate();
	virtual void Draw();
	virtual void DrawGUI() {};
	virtual Tag GetTag();
	virtual void SetTag(GameObject::Tag tagPar);
	virtual void Move(float speed, irr::core::vector3df direction, bool turnToDirection = false);
	void NotifyCollision(irr::core::vector3df objPosition);

private:
};
