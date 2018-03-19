#pragma once
#include <irrlicht.h>

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

class GameObject
{
public:
	// Constructor
	GameObject(IAnimatedMeshSceneNode* relatedNode, ITexture* relatedTexture,
		const vector3df* startPosition = new vector3df(0, 0, 0),
		const vector3df* startScale = new vector3df(1, 1, 1),
		const vector3df* startRotation = new vector3df(0, 0, 0));
	// Constructor
	GameObject(const vector3df* startPosition = new vector3df(0, 0, 0),
		const vector3df* startScale = new vector3df(1, 1, 1),
		const vector3df* startRotation = new vector3df(0, 0, 0));
	// Destructor
	~GameObject();

	// The linked node for this GameObject
	IAnimatedMeshSceneNode* node;

	// Functions
	void Update();
	void Draw();
};