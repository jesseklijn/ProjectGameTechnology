#pragma once
#include "GameObject.h" 
#pragma once
#include "GameManager.h" 

// Constructor
GameObject::GameObject(const irr::core::vector3df* startPosition,
						const irr::core::vector3df* startScale,
						const irr::core::vector3df* startRotation, 
						irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,						
						irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision,
						float mass)
						: PhysicsObject(parent, mgr, id, startPosition, mass)
{
	// Set the position, scale and rotation of our GameObject
	setPosition(*startPosition);
	setScale(*startScale);
	setRotation(*startRotation);

	// If a mesh and texture were given
	if (relatedMesh && relatedTexture)
	{
		// Set mesh details
		mesh = GameManager::smgr->addAnimatedMeshSceneNode(relatedMesh, 0);
		if (mesh)
		{
			PhysicsObject::mesh = mesh;
		}

		/* Set some default visual values for the node
		TODO: Add to constructor?*/
		mesh->setMaterialFlag(EMF_LIGHTING, true);
		mesh->setMaterialFlag(EMF_FOG_ENABLE, true);
		mesh->setMD2Animation(scene::EMAT_STAND);
		mesh->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		mesh->setMaterialTexture(0, relatedTexture);

		// Set the position, scale and rotation of our mesh
		mesh->setPosition(*startPosition);
		mesh->setScale(*startScale);
		mesh->setRotation(*startRotation);

		/* Create selection functionality so raycasts will detect it
		Initialize a base selector, used for assigning selection to scene nodes
		It's dropped after every selector assignment, but it's easily re-usable*/
		if (detectCollision)
		{
			selector = GameManager::smgr->createTriangleSelector(mesh);
			mesh->setTriangleSelector(selector);
			selector->drop();
		}
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
	// Inherit base class Update
	PhysicsObject::Update();
	// Run the Update() of our base class
	DynamicUpdater::Update();

	updateAbsolutePosition();
}

// Draw
void GameObject::Draw()
{

}

void GameObject::Move(float speed, irr::core::vector3df direction, bool turnToDirection)
{
	// Add a vector of length speed in the given direction
	//setPosition(getPosition() + (direction.normalize() * speed));
	//if (mesh)
	//	mesh->setPosition(getPosition());
	//if (turnToDirection)
	//{
	//	setRotation(direction.getHorizontalAngle());
	//	if (mesh)
	//		mesh->setRotation(direction.getHorizontalAngle());
	//}

	PhysicsObject::addForce(0.001 * moveSpeed * direction.normalize());
	if (turnToDirection)
	{
		PhysicsObject::turnToDirection(direction);
	}
}

GameObject::Tag GameObject::GetTag()
{
	return tag;
}
