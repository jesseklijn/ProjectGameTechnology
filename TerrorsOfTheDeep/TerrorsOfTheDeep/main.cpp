#pragma region Includes
#pragma once
#include <irrlicht.h>
#pragma once
#include "Lighting.h"
#pragma once
#include "HUD.h"
#pragma once
#include "Player.h"
#pragma once
#include "Camera.h"
#pragma once
#include "DetectCollision.h"
#pragma once
#include "Sound.h"
#pragma once
#include "GameManager.h"
#pragma once
#include "GameObject.h"
#pragma once
#include "Shark.h"
#pragma once
#include "Dolphin.h"
#pragma once
#include "Goldback.h"
#pragma once
#include "Bass.h"
#pragma once
#include <string>
#pragma once

#include "FlockingEntity.h"
#pragma endregion
#include <chrono>

#pragma region Namespaces
// Main namespace
using namespace irr;

// Namespaces of Irrlicht
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

#pragma region Variables
// TODO: Place these in GameObjects where they belong

// Stamina variable for swimming faster etc
int stamina = 0;

// Check if the items are picked up
bool itemPickedUp[3] = { false, false, false };

// Light colours
irr::video::SColorf ambientColor = irr::video::SColorf(0.1f,0.1f,0.1f,0.1f);
//irr::video::SColorf ambientColor = irr::video::SColorf(1, 1, 1, 1);	// for when you need to see stuff
irr::video::SColorf flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);
const float FLASHLIGHT_RANGE = 1000.f;

// Create HUD object
HUD* hud = new HUD;

// Whether to hide or show the HUD
bool disableHud = false;
float rockMass = 500;
int dolphinCount = 3;
int goldbackCount = 50;
int bassCount = 50;
#pragma endregion


/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	/* Seed the random number generator, so we don't end up with
	the same random numbers on every run */
	srand(static_cast<unsigned>(time(0)));

	// Create a GameManager, set window caption and hide our mouse
	GameManager gameManager;
	GameManager::device->setWindowCaption(L"Terrors of the Deep - Vertical Slice");
	GameManager::device->getCursorControl()->setVisible(false);

	// Set our skydome
	ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);
	skydome->setMaterialFlag(EMF_FOG_ENABLE, true);

	// Initialize our background music
	sound_init();
	background_music("../media/AmbientUnderwaterMaddnes.ogg");

	// Adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(GameManager::smgr);
	GameManager::smgr->getActiveCamera()->setPosition(vector3df(20, 0, 20));
	Lighting lighting = Lighting(GameManager::smgr);

	// Ambient Scene Light
	lighting.SetSceneLight(ambientColor);
	// Set and attach flashlight to player
	//ISceneNode *playerNode = &player;

	// Spawn critters
	for (int dolphinIndex = 0; dolphinIndex < dolphinCount; ++dolphinIndex)
	{
		Dolphin* dolphin = new Dolphin(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX, 
			rand() % GameManager::worldRadiusY, 
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1), new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh("../media/dolphin.obj"),
			GameManager::driver->getTexture("../media/skydome.jpg"), false);
		GameManager::gameObjects.push_back(dolphin);
	}


	for (int goldbackIndex = 0; goldbackIndex < goldbackCount; ++goldbackIndex)
	{
		Goldback* goldbackFish = new Goldback(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			rand() % GameManager::worldRadiusY,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1), new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh("../media/GoldenFish.obj"),
			GameManager::driver->getTexture("../media/naranjaojo.png"), false);		
		GameManager::gameObjects.push_back(goldbackFish);
	}

	for (int bassIndex = 0; bassIndex < bassCount; ++bassIndex)
	{
		Bass* bassFish = new Bass(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			rand() % GameManager::worldRadiusY,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1), new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh("../media/Rudd_Fish.obj"),
			GameManager::driver->getTexture("../media/Rudd-Fish_Colourmap.png"), false);
		GameManager::gameObjects.push_back(bassFish);
	}

	Shark* shark = new Shark(new vector3df(4000, 50, 0), new vector3df(50, 50, 50), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1111,
		GameManager::smgr->getMesh("../media/shark.obj"),
		GameManager::driver->getTexture("../media/Shark_Texture.jpg"), false);
	GameManager::gameObjects.push_back(shark);

	IAnimatedMesh* playerMesh = GameManager::smgr->getMesh("../media/FPS_Arms.obj");
	// Player
	Player* player = new Player(new vector3df(0, -0.8, 1.5), new vector3df(4, 4, 4), new vector3df(0, 0, 0),
		GameManager::smgr->getActiveCamera(), GameManager::smgr, 9000, playerMesh, 
		GameManager::driver->getTexture("../media/armsTexture.png"));
	GameManager::gameObjects.push_back(player);

	ISceneNode* newPlayer = player;
	ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor, player->getAbsolutePosition(), GameManager::smgr->getActiveCamera()->getTarget(), FLASHLIGHT_RANGE, true, player);
	//ILightSceneNode* flashlight = lighting.CreatePointLight(flashlightColor, player->getAbsolutePosition(), false, player);
	//ILightSceneNode* eyeRight = lighting.CreateDirectionalLight(sharkEyesColor, shark->getPosition(), shark->getRotation(), 200.f,false, shark);
	//ILightSceneNode* eyeLeft = lighting.CreateDirectionalLight(sharkEyesColor, vector3df(shark->getPosition().X+50, shark->getPosition().Y+10, shark->getPosition().Z-50), shark->getRotation(), 200.f, false, shark);


	FlockingEntity* flockOfFish = new FlockingEntity(new vector3df(100,-80, 100), new vector3df(5, 5, 5), new vector3df(0, 0, 0),
		GameManager::smgr->getRootSceneNode(), GameManager::smgr, -500, GameManager::smgr->getMesh("../media/FishSpawn.obj"),
		GameManager::driver->getTexture("../media/GoldTexture.jpg"));
	flockOfFish->tag = GameObject::CREATURE;
	GameManager::gameObjects.push_back(flockOfFish);

	

	// Rock
	GameObject* rock = new GameObject(new vector3df(-400, -50, 100), new vector3df(150, 150, 150), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1112,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"), true, rockMass);
	rock->tag = GameObject::WORLD_OBJECT;
	GameManager::gameObjects.push_back(rock);

	GameObject* rock1 = new GameObject(new vector3df(-400, -40, -200), new vector3df(150, 150, 150), new vector3df(0, 0, 0),
		0, GameManager::smgr, 3,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"), true, rockMass);
	rock1->tag = GameObject::WORLD_OBJECT;
	GameManager::gameObjects.push_back(rock1);

	GameObject* rock2 = new GameObject(new vector3df(-750, -40, -400), new vector3df(120, 120, 120), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1114,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"), true, rockMass);
	rock2->tag = GameObject::WORLD_OBJECT;
	GameManager::gameObjects.push_back(rock2);

	GameObject* rock3 = new GameObject(new vector3df(-700, -50, 300), new vector3df(100, 100, 100), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1115,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"), true, rockMass);
	rock3->tag = GameObject::WORLD_OBJECT;
	GameManager::gameObjects.push_back(rock3);

	GameObject* rock4 = new GameObject(new vector3df(-1000, -40, 205), new vector3df(150, 150, 150), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1116,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"), true, rockMass);
	rock4->tag = GameObject::WORLD_OBJECT;
	GameManager::gameObjects.push_back(rock4);

	GameObject* groundPlane = new GameObject(new vector3df(100, -100, 0), new vector3df(10000, 1, 10000), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1116,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/SandTexture.jpg"));
	groundPlane->tag = GameObject::GROUND;
	GameManager::gameObjects.push_back(groundPlane);

	// Key collectible object
	GameObject* key = new GameObject(new vector3df(-725, -70, 0), new vector3df(0.5, 0.5, 0.5), new vector3df(0, 0, 0),
		0, GameManager::smgr, 4,
		GameManager::smgr->getMesh("../media/key.obj"),
		GameManager::driver->getTexture("../media/RustTexture.jpg"));
	key->tag = GameObject::KEY;
	GameManager::gameObjects.push_back(key);
	ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.2f, 0.5f, 1.f), key->getPosition(), 200.f, key->getRotation(), false, key);

	// Win Condition trigger object
	GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
		0, GameManager::smgr, 5,
		GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
		GameManager::driver->getTexture("../media/GoldTexture.jpg"));
	chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	chest->tag = GameObject::CHEST;
	GameManager::gameObjects.push_back(chest);


	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Delta time start point
		auto frameTimeStart = std::chrono::system_clock::now();

		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
		//!!Change parameters to correct type


		// Update our scene. gameManager.Update will also call Update for all GameObjects and their linked nodes
		gameManager.Update();

		//check the boundaries
		camera.updatePos();

		Detect(itemPickedUp);

		// We finished changing the scene
		// Now draw the scene in our actual window
		GameManager::smgr->drawAll();

		// Clear the HUD, update HUD values and prepare the updated HUD
		GameManager::guienv->clear();

		if (stamina >= 0 && stamina < 1000) {
			stamina++;
		}
		if (!disableHud) {
			hud->HudDraw(stamina, itemPickedUp, GameManager::driver, GameManager::guienv);
		}

		// Run the Draw() of the GameManager, which in turn also runs the Draw() for all GameObjects and their linked scene nodes
		gameManager.Draw();

		// Finally, draw our HUD on the screen		
		GameManager::guienv->drawAll();

		// Our frame is finished
		GameManager::driver->endScene();

		// Delta time end point, calculate time passed for the next frame
		auto frameTimeEnd = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_seconds = frameTimeEnd - frameTimeStart;
		GameManager::deltaTime = elapsed_seconds.count();
		GameManager::deltaTimeMS = GameManager::deltaTime * 1000.0f;
		GameManager::time += elapsed_seconds.count();
	}
	// Game end, drop our Irrlicht device
	GameManager::device->drop();

	// Stop sound
	sound_shutdown();

	return 0;
}
