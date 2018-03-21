#pragma once
#include "irrlicht.h"


class GameObject : public irr::scene::ISceneNode
{
public:
	// Constructor
	GameObject(const irr::core::vector3df* startPosition,
				const irr::core::vector3df* startScale,
				const irr::core::vector3df* startRotation, 
				irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
				irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);

	// Destructor
	~GameObject();

	// The linked node for this GameObject
	irr::scene::IAnimatedMeshSceneNode* mesh;

	// ISceneNode stuff
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::SMaterial Material;

	// Functions
	virtual void render();
	virtual void OnRegisterSceneNode();
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	virtual void Update();
	virtual void Draw();
};
