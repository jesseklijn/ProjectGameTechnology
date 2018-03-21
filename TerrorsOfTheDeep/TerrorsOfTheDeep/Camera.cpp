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
	// const float sidewaysSpeed = (float)0.1;

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
	smgr->addCameraSceneNodeFPS(0, ROTATION_SPEED, MOVEMENT_SPEED, -100, keyMap, 4);
}

// Destructor
Camera::~Camera()
{

}

// In main:
// Camera camera = Camera(smgr);
