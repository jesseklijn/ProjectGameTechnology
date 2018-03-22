#pragma region Includes
#pragma once
#include <irrlicht.h>
#pragma once
#include "addLighting.h"
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
#include <string>
#pragma endregion

#pragma region Namespaces
//Main namespace
using namespace irr;

//Namespaces of Irrlicht
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

// Create HUD object
HUD* hud = new HUD;

// Whether to hide or show the HUD
bool disableHud = false;
#pragma endregion


/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	// Create a GameManager, set window caption and hide our mouse
	GameManager gameManager;
	GameManager::device->setWindowCaption(L"Terrors of the Deep - Vertical Slice");
	GameManager::device->getCursorControl()->setVisible(false);

	// Set our skybox
	ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);

	// Initialize our background music
	sound_init();
	background_music("../media/JawsTheme.ogg");

	// Adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(GameManager::smgr);

	/* Create dummy objects for testing
	Shark*/	
	Shark* shark = new Shark(new vector3df(80, 0, 120), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1111,
		GameManager::smgr->getMesh("../media/shark.obj"),
		GameManager::driver->getTexture("../media/Shark_Texture.jpg"));
	shark->tag = "Monster";

	/* TODO: Find a way to integrate this in derived (child) classes.

	It was in GameObject base class first, but that would register the GameObject obviously.
	Here we create a derived class instance of Monster, which derives from GameObject.

	We now use a vector array in GameManager, which can hold multiple different
	class types, which means we can add children of GameObject! Since we add a Monster
	and not a GameObject here, we make the list loop run Update() for the Monster instance,
	not the GameObject.

	Long story short: Add the line below if your own object class:
	- Inherits from GameObject
	- Is using an Update() function
	*/
	GameManager::gameObjects.push_back(shark);

	// Player
	Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111);
	player->tag = "Player";
	GameManager::gameObjects.push_back(player);


	// Rock
	GameObject* rock = new GameObject(new vector3df(0, 20, 0), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1112,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));
	rock->tag = "World Object";
	GameManager::gameObjects.push_back(rock);

	GameObject* rock1 = new GameObject(new vector3df(-200, -50, 75), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
		0, GameManager::smgr, 3,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	GameObject* rock2 = new GameObject(new vector3df(0, -50, -25), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1114,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	GameObject* rock3 = new GameObject(new vector3df(-300, -50, 300), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1115,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	GameObject* rock4 = new GameObject(new vector3df(-375, -50, 205), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
		0, GameManager::smgr, -1116,
		GameManager::smgr->getMesh("../media/rock.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	// Key collectible object
	GameObject* key = new GameObject(new vector3df(-175, -40, 100), new vector3df(0.1, 0.1, 0.1), new vector3df(0, 0, 0),
		0, GameManager::smgr, 4,
		GameManager::smgr->getMesh("../media/key.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	// Win Condition trigger object
	GameObject* win = new GameObject(new vector3df(-250, -40, 300), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		0, GameManager::smgr, 5,
		GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
		GameManager::driver->getTexture("../media/RockTexture.jpg"));

	ISceneNode* newPlayer = player;

	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
		//!!Change parameters to correct type


		// Update our scene. gameManager.Update will also call Update for all GameObjects and their linked nodes
		gameManager.Update();		

		//check the boundaries
		camera.updatePos();

		Detect(newPlayer,
			win->mesh,
			key->mesh,
			shark->mesh,
			rock->mesh,
			rock1->mesh,
			rock2->mesh,
			rock3->mesh,
			rock4->mesh,
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
	}
	// Game end, drop our Irrlicht device
	GameManager::device->drop();

	// Stop sound
	sound_shutdown();

	return 0;
}
