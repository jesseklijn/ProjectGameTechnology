#pragma once
#include <irrlicht.h>
#include "GameObject.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Defines a player character to interact with the game world.
class Player : public GameObject
{
public:
	// Constructor and deconstructor
	Player(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		float movementSpeed,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	virtual ~Player();

	// Variables
	irr::IrrlichtDevice* irrDevice;
	irr::scene::ISceneManager* smgr;
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::S3DVertex vertices[8];
	irr::video::SMaterial Material;

	float speed;
	bool canMove = false;

	// used?
	irr::core::vector3df latestPos;

	bool isColliding;
	irr::core::vector3df colPos;


	// Functions
	virtual void Update();
	virtual void render();
	virtual void OnRegisterSceneNode();
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	void UpdatePos();
	void MoveParent(irr::core::vector3df movement);

	void NotifyCollision(irr::core::vector3df objPosition);
};
