#pragma once
#include <irrlicht.h>
#pragma once
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
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~Player();

	// Variables
	irr::IrrlichtDevice* irrDevice;
	irr::scene::ISceneManager* smgr;
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::S3DVertex Vertices[8];
	irr::video::SMaterial Material;
	irr::u32 then;
	irr::core::vector3df latestPos;
	float deltaX;
	float deltaZ;

	// int oxygen, stamina...

	// Functions
	virtual void Update();
	virtual void render();
	virtual void OnRegisterSceneNode();
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	void UpdatePos();
	void MoveArms(float speed);
};

