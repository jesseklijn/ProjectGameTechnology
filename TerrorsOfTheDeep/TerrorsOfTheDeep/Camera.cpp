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
	const float MOVEMENT_SPEED = 1.5f;
	const float ROTATION_SPEED = 100.0f;

	// Add a camera in the scene
	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, ROTATION_SPEED, MOVEMENT_SPEED, -100);
	camera->setFarValue(20000.0f);
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
	// Add a new main camera in the scene
	camera = GameManager::smgr->addCameraSceneNodeFPS(parent, rotationSpeed, moveSpeed, id, keyMapArray, 
		keyMapArray != 0 ? sizeof(keyMapArray) : 0, noVertMovement, jumpSpeed, invertMouse, makeActive);
	GameManager::smgr->setActiveCamera(camera);
	camera->setPosition(position);
	camera->setTarget(lookTarget);
	camera->setFarValue(farValue);

	// Parent the player to it
	if (GameManager::levelPlayer != nullptr)
	{
		GameManager::levelPlayer->setParent(camera);

		/* Once the player's previous parent (last camera) has changed, apparently the player's mesh
		also loses its player parent for some reason. Re-initialize it. */
		if (GameManager::levelPlayer->mesh)
			GameManager::levelPlayer->mesh->setParent(GameManager::levelPlayer);
	}

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
