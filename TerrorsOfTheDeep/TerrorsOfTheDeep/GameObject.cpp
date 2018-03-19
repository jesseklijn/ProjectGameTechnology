#pragma once
#include "GameObject.h" 
#pragma once
#include "GameManager.h" 

// Constructor
GameObject::GameObject(irr::scene::IAnimatedMeshSceneNode* relatedNode, irr::video::ITexture* relatedTexture,
						const irr::core::vector3df* startPosition,
						const irr::core::vector3df* startScale,
						const irr::core::vector3df* startRotation)
{
	// Link the given node with this GameObject
	node = relatedNode;

	// Set some default visual values for the node
	// TODO: Add to constructor?
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMD2Animation(scene::EMAT_STAND);
	node->setMaterialTexture(0, relatedTexture);

	// Set the position, scale and rotation of our node
	node->setPosition(*startPosition);
	node->setScale(*startScale);
	node->setRotation(*startRotation);

	// Add this GameObject to the GameManager, mainly for remote Update() and Draw() calls
	GameManager::gameObjects->push_back(*this);
}

GameObject::GameObject(const vector3df * startPosition, const vector3df * startScale, const vector3df * startRotation)
{
}

// Destructor
GameObject::~GameObject()
{

}

// Update
void GameObject::Update()
{

}

// Draw
void GameObject::Draw()
{

}