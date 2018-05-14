#pragma once
#include <irrlicht.h>
#include "GameManager.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Class that initializes a main camera.
class Camera
{
public:
	explicit Camera(irr::scene::ISceneManager* smgr);	
	~Camera();

	float cameraSpeed = 0.4f;
	float cameraRotationSpeed = 100.0f;
	float cameraFarValue = 20000.0f;
	SKeyMap keyMap[4];
	ICameraSceneNode* camera;

	ICameraSceneNode* CreateCamera(vector3df position, vector3df lookTarget, float rotationSpeed, float moveSpeed, float farValue = 20000.0f,
		irr::SKeyMap* keyMapArray = (irr::SKeyMap*)0, ISceneNode* parent = 0, int id = -1, bool noVertMovement = false, 
		float jumpSpeed = 0.0f, bool invertMouse = false, bool makeActive = true);
	void updatePos();
};
