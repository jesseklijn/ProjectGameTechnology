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
#include "Critter.h"
#pragma once
#include "Shark.h"
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
irr::video::SColorf ambientColor = irr::video::SColorf(1.0f,1.0f,1.0f,1.0f);
irr::video::SColorf flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);

// Create HUD object
HUD* hud = new HUD;

// Whether to hide or show the HUD
bool disableHud = false;

// Scenery generation
int critterCount = 350;
int shipCount = 6;
int rockCount = 60;
int ruinsCount = 25;
int coralCount = 75;
int plantCount = 25;
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
	Lighting lighting = Lighting(GameManager::smgr);
	lighting.SetSceneLight(ambientColor);

	// Spawn the ground
	GameObject* groundPlane = new GameObject(new vector3df(0, -100, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1116,
		GameManager::smgr->getMesh("../media/plane.obj"),
		0, false);

	std::vector<io::path> meshDirectories;
	std::vector<io::path> meshTextures;

	// Spawn critters
	meshDirectories.push_back("../media/dolphin.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Fish1.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Fish2.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Fish3.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/MantaRay.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/GoldenFish.obj"); meshTextures.push_back("../media/naranjaojo.png");
	meshDirectories.push_back("../media/Rudd_Fish.obj"); meshTextures.push_back("../media/Rudd-Fish_Colourmap.png");
	for (int critterIndex = 0; critterIndex < critterCount; critterIndex++)
	{
		int graphicsIndex = rand() % meshDirectories.size();
		Critter* critter = new Critter(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			rand() % GameManager::worldRadiusY,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1), 
			new vector3df(0, 0, 0),
			0, 
			GameManager::smgr, 
			-1111,
			GameManager::smgr->getMesh(meshDirectories[graphicsIndex]),
			meshTextures[graphicsIndex] != "" ? GameManager::driver->getTexture(meshTextures[graphicsIndex]) : 0, 
			false);
		GameManager::gameObjects.push_back(critter);
	}

	// Spawn world objects
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/ship.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/SailShip.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Boat.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/BoatWSail.obj"); meshTextures.push_back("");
	for (int shipIndex = 0; shipIndex < shipCount; shipIndex++)
	{
		int graphicsIndex = rand() % meshDirectories.size();
		GameObject* ship = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			-100,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1),
			new vector3df(rand() % 25, rand() % 10, rand() % 10),
			0, 
			GameManager::smgr, 
			-1111,
			GameManager::smgr->getMesh(meshDirectories[graphicsIndex]),
			meshTextures[graphicsIndex] != "" ? GameManager::driver->getTexture(meshTextures[graphicsIndex]) : 0);
		GameManager::gameObjects.push_back(ship);
	}

	// Spawn ruins
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/ruinsArc.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsCathedral.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsFoundation.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsPillar.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsTempleRuin1.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsTempleRuin2.obj"); meshTextures.push_back("");
	for (int ruinsIndex = 0; ruinsIndex < ruinsCount; ruinsIndex++)
	{
		int typeIndex = rand() % meshDirectories.size();
		GameObject* ruin = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			-100,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh(meshDirectories[typeIndex]),
			0);
		GameManager::gameObjects.push_back(ruin);
	}

	// Spawn coral
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Coral/coralBrain1.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralBrain2.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralBrain3.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralPillar.obj"); meshTextures.push_back("");
	for (int coralIndex = 0; coralIndex < ruinsCount; coralIndex++)
	{
		int typeIndex = rand() % meshDirectories.size();
		GameObject* coral = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			-100,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh(meshDirectories[typeIndex]),
			0);
		for (int mIndex = 0; mIndex < coral->mesh->getMaterialCount(); mIndex++)
			coral->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		GameManager::gameObjects.push_back(coral);
	}

	// Spawn vines
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Plants/tiny_weed_03_01.obj"); meshTextures.push_back("");
	for (int plantIndex = 0; plantIndex < plantCount; plantIndex++)
	{
		int typeIndex = rand() % meshDirectories.size();
		GameObject* vines = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			-100,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh(meshDirectories[typeIndex]),
			0);
		for (int mIndex = 0; mIndex < vines->mesh->getMaterialCount(); mIndex++)
			vines->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		GameManager::gameObjects.push_back(vines);
	}

	// Spawn rocks
	for (int rockIndex = 0; rockIndex < rockCount; rockIndex++)
	{
		GameObject* rock = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
			-100,
			rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
			new vector3df(150 + (rand() % 150) * 0.25f, 150 + (rand() % 150) * 0.25f, 150 + (rand() % 150) * 0.25f),
			new vector3df(rand() % 360, rand() % 360, rand() % 360),
			0, GameManager::smgr, -1111,
			GameManager::smgr->getMesh("../media/Rock.obj"),
			0);
		for (int mIndex = 0; mIndex < rock->mesh->getMaterialCount(); mIndex++)
			rock->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		GameManager::gameObjects.push_back(rock);
	}

	// SPawn massive skull
	GameObject* skull = new GameObject(new vector3df(rand() % (GameManager::worldRadiusX * 2) - GameManager::worldRadiusX,
		-100,
		rand() % (GameManager::worldRadiusZ * 2) - GameManager::worldRadiusZ),
		new vector3df(1, 1, 1),
		new vector3df(0, 0, 0),
		0, GameManager::smgr, -1111,
		GameManager::smgr->getMesh("../media/Bones/skullBig.obj"),
		0);
	skull->mesh->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
	GameManager::gameObjects.push_back(skull);

	// Spawn player
	Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111);
	GameManager::gameObjects.push_back(player);

	// Attach flashlight to player
	ISceneNode* newPlayer = player;
	ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor, newPlayer->getPosition(), GameManager::smgr->getActiveCamera()->getTarget(), 5.0f, true, newPlayer);

	// Spawn shark
	Shark* shark = new Shark(new vector3df(4000, 500, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1111,
		GameManager::smgr->getMesh("../media/Shark.obj"),
		0, false);
	GameManager::gameObjects.push_back(shark);

	FlockingEntity* flockOfFish = new FlockingEntity(new vector3df(100,-80, 100), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		GameManager::smgr->getRootSceneNode(), GameManager::smgr, -500, GameManager::smgr->getMesh("../media/FishSpawn.obj"),
		GameManager::driver->getTexture("../media/GoldTexture.jpg"));
	flockOfFish->tag = GameObject::CREATURE;
	GameManager::gameObjects.push_back(flockOfFish);

	// Key collectible object
	GameObject* key = new GameObject(new vector3df(-725, -70, 0), new vector3df(0.5, 0.5, 0.5), new vector3df(0, 0, 0),
		0, GameManager::smgr, 4,
		GameManager::smgr->getMesh("../media/key.obj"),
		GameManager::driver->getTexture("../media/RustTexture.jpg"));
	key->tag = GameObject::KEY;
	GameManager::gameObjects.push_back(key);

	// Win condition trigger object
	GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
		0, GameManager::smgr, 5,
		GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
		GameManager::driver->getTexture("../media/GoldTexture.jpg"));
	chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, false);
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

		Detect(newPlayer,
			chest->mesh,
			key->mesh,
			shark->mesh,
			itemPickedUp,
			GameManager::smgr);

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
