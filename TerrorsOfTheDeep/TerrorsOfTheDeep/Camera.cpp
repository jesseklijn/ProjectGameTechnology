#pragma once
#include "Camera.h"
#include "SceneManager.h"

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

// Constructor
Camera::Camera(ISceneManager* smgr)
{
	// Keymapping for player controls
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;

	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;

	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;

	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;
}

// Destructor
Camera::~Camera()
{

}


ICameraSceneNode * Camera::CreateCamera(vector3df position, 
	vector3df lookTarget, float rotationSpeed, float moveSpeed, 
	float farValue, irr::SKeyMap * keyMapArray, ISceneNode * parent, 
	int id, bool noVertMovement, float jumpSpeed, bool invertMouse, 
	bool makeActive)
{
	ICameraSceneNode* currentCam = GameManager::smgr->getActiveCamera();
	if (currentCam)
	{
		GameManager::smgr->setActiveCamera(0);
		camera->remove();
		camera = nullptr;
	}	

	// Add a camera in the scene
	camera = GameManager::smgr->addCameraSceneNodeFPS(parent, rotationSpeed, moveSpeed, id, keyMapArray, 
		keyMapArray != 0 ? sizeof(keyMapArray) : 0, noVertMovement, jumpSpeed, invertMouse, makeActive);
	camera->setPosition(position);
	camera->setTarget(position + lookTarget);
	camera->setFarValue(farValue);

	if (SceneManager::levelPlayer != nullptr)
		SceneManager::levelPlayer->setParent(camera);

	return camera;
}

//check boundaries
void Camera::updatePos()
{
	if (!GameManager::smgr->getActiveCamera())
		return;

	if (GameManager::smgr->getActiveCamera()->getPosition().X > GameManager::WORLD_RADIUS_X)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::WORLD_RADIUS_X - 1, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().X < -GameManager::WORLD_RADIUS_X)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(-GameManager::WORLD_RADIUS_X + 1, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Y > GameManager::WORLD_RADIUS_Y)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::WORLD_RADIUS_Y - 1,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Y < -50)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, -49,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Z > GameManager::WORLD_RADIUS_Z)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::WORLD_RADIUS_Z - 1));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Z < -GameManager::WORLD_RADIUS_Z)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::smgr->getActiveCamera()->getPosition().Y,
			-GameManager::WORLD_RADIUS_Z + 1));
	}
}

// In main:
// Camera camera = Camera(smgr);
