#pragma once
#include "GameObject.h" 
#pragma once
#include "GameManager.h" 

// Constructor
GameObject::GameObject(const irr::core::vector3df* startPosition,
						const irr::core::vector3df* startScale,
						const irr::core::vector3df* startRotation, 
						irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,						
						irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture) : ISceneNode(parent, mgr, id)
{
	// Position, scale and rotation setting
	setPosition(*startPosition);
	setScale(*startScale);
	setRotation(*startRotation);

	// If a mesh and texture were given
	if (relatedMesh && relatedTexture)
	{
		// Set mesh details
		mesh = GameManager::smgr->addAnimatedMeshSceneNode(relatedMesh, parent);

		// Set some default visual values for the node
		// TODO: Add to constructor?
		mesh->setMaterialFlag(EMF_LIGHTING, false);
		mesh->setMD2Animation(scene::EMAT_STAND);
		mesh->setMaterialTexture(0, relatedTexture);

		// Set the position, scale and rotation of our mesh
		mesh->setPosition(*startPosition);
		mesh->setScale(*startScale);
		mesh->setRotation(*startRotation);
	}
}

// Destructor
GameObject::~GameObject()
{

}

void GameObject::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void GameObject::render()
{

}

const aabbox3d<f32>& GameObject::getBoundingBox() const
{
	return Box;
}

u32 GameObject::getMaterialCount() const
{
	return 1;
}

SMaterial& GameObject::getMaterial(u32 i)
{
	return Material;
}

// Update
void GameObject::Update()
{

}

// Draw
void GameObject::Draw()
{

}