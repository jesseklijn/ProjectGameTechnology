#pragma once
#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Defines a player character to interact with the game world.
class Player : public irr::scene::ISceneNode
{
public:
	irr::IrrlichtDevice* irrDevice;
	irr::scene::ISceneManager* smgr;
	irr::scene::ISceneNode* parentNode;

	irr::core::aabbox3d<irr::f32> Box;
	irr::video::S3DVertex Vertices[8];
	irr::video::SMaterial Material;

	irr::core::vector3df latestPos;
	irr::core::vector3df latestRot;

	irr::u32 then;
	float deltaX;
	float deltaZ;

	Player(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id, irr::IrrlichtDevice* device);
	~Player();

	virtual void render();
	virtual void OnRegisterSceneNode();
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	void UpdatePos();
	void MoveArms(float speed);
};

