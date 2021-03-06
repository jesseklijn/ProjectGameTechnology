#include "GameObject.h" 
#include "GameManager.h" 

// Constructor
GameObject::GameObject(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision, bool castsShadows, float mass)
						: PhysicsObject(parent, mgr, id, startPosition, mass)

{
	SetTag(GameObject::WORLD_OBJECT);

	setPosition(*startPosition);
	setScale(*startScale);
	setRotation(*startRotation);

	// If a mesh and texture were given
	if (relatedMesh)
	{
		// Set mesh details
		mesh = GameManager::smgr->addAnimatedMeshSceneNode(relatedMesh, parent);
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

		if (relatedTexture)
			mesh->setMaterialTexture(0, relatedTexture);

		// Set the position, scale and rotation of our mesh
		mesh->setPosition(*startPosition);
		mesh->setScale(*startScale);
		mesh->setRotation(*startRotation);

		if (castsShadows)
			mesh->addShadowVolumeSceneNode();

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
	// Clear GameManager tracking list entry
	int oIndex = GameManager::FindIndexInList<GameObject>(this, GameManager::gameObjects);
	if (oIndex != -1)
		GameManager::gameObjects[oIndex] = nullptr;
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
	return box;
}

u32 GameObject::getMaterialCount() const
{
	return 1;
}

SMaterial& GameObject::getMaterial(u32 i)
{
	return material;
}

/** Runs a default Update() loop, ran every frame.

NOTE: In order to make something framerate independent, make use of delta timing.
It's accessible in GameManager as GameManager::deltaTime and GameManager::deltaTimeMS
*/
void GameObject::Update()
{
	// Inherit base class Update
	PhysicsObject::Update();

	// Run the Update() of our base class
	DynamicUpdater::Update();

	isColliding = false;

	updateAbsolutePosition();
}

/** Runs a FixedUpdate() loop, meant for physics related activities.

NOTE: use of GameManager::deltaTimeFixed and GameManager::deltaTimeFixedMS is
recommended here, which is adjusted to the fixed physics time step defined in GameManager.
*/
void GameObject::FixedUpdate()
{
}

// Draw
void GameObject::Draw()
{

}

void GameObject::Move(float speed, irr::core::vector3df direction, bool turnToDirection)
{
	//PhysicsObject::AddForce(0.0001 * moveSpeed * direction.normalize());
	//if (turnToDirection)
	//{
	//	PhysicsObject::TurnToDirection(direction);
	//}

	//Add a vector of length speed in the given direction

	vector3df newPos = getPosition() + (direction.normalize() * speed);
	if (!(isColliding && getPosition().getDistanceFrom(colPos) > newPos.getDistanceFrom(colPos)))
		setPosition(newPos);

	if (mesh)
		mesh->setPosition(getPosition());


	if (turnToDirection)
	{
		setRotation(direction.getHorizontalAngle());
		if (mesh)
			mesh->setRotation(direction.getHorizontalAngle());
	}
}

GameObject::Tag GameObject::GetTag()
{
	return tag;
}

void GameObject::SetTag(GameObject::Tag tagPar)
{
	tag = tagPar;
	PhysicsObject::tag = tag;
}

void GameObject::NotifyCollision(vector3df objPosition)
{
	isColliding = true;
	colPos = objPosition;
}
