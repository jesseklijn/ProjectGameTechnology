#pragma once
#include "irrlicht.h"

class GameObject
{
public:
	// Constructor
	GameObject(irr::scene::IAnimatedMeshSceneNode* relatedNode, irr::video::ITexture* relatedTexture, 
						const irr::core::vector3df* startPosition = new irr::core::vector3df(0, 0, 0), 
						const irr::core::vector3df* startScale = new irr::core::vector3df(1, 1, 1), 
						const irr::core::vector3df* startRotation = new irr::core::vector3df(0, 0, 0));
	// Destructor
	~GameObject();

	// The linked node for this GameObject
	irr::scene::IAnimatedMeshSceneNode* node;

	// Functions
	void Update();
	void Draw();
};