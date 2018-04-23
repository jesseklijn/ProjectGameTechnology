#pragma once
#include "Camera.h"

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
	const float MOVEMENT_SPEED = (float)0.25;
	const float ROTATION_SPEED = (float)25;

	// Keymapping for player controls
	SKeyMap keyMap[4];

	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;

	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;

	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;

	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;

	// Add a camera in the scene
	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, ROTATION_SPEED, MOVEMENT_SPEED, -100, keyMap, 4);
	camera->setFarValue(8000.0f);
}

// Destructor
Camera::~Camera()
{

}

//check boundaries
void Camera::updatePos()
{
	if (GameManager::smgr->getActiveCamera()->getPosition().X > GameManager::worldRadiusX)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::worldRadiusX - 1, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().X < -GameManager::worldRadiusX)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(-GameManager::worldRadiusX + 1, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Y > GameManager::worldRadiusY)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::worldRadiusY - 1,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Y < -50)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, -49,
			GameManager::smgr->getActiveCamera()->getPosition().Z));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Z > GameManager::worldRadiusZ)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::smgr->getActiveCamera()->getPosition().Y,
			GameManager::worldRadiusZ - 1));
	}
	if (GameManager::smgr->getActiveCamera()->getPosition().Z < -GameManager::worldRadiusZ)
	{
		GameManager::smgr->getActiveCamera()->setPosition(vector3df(GameManager::smgr->getActiveCamera()->getPosition().X, GameManager::smgr->getActiveCamera()->getPosition().Y,
			-GameManager::worldRadiusZ + 1));
	}
}

// In main:
// Camera camera = Camera(smgr);
