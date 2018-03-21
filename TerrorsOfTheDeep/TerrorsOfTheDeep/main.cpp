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
#include "Monster.h"
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
bool itemPickedUp[3] = { true, true, true };

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
	ISceneNode* skybox = GameManager::smgr->addSkyBoxSceneNode(
		GameManager::driver->getTexture("../media/irrlicht2_up.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_dn.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_lf.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_rt.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_ft.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_bk.jpg"));
	ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);

	// Initialize our background music
	sound_init();
	background_music("../media/JawsTheme.ogg");
		
	// Adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(GameManager::smgr);
	Player player = Player(GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111, GameManager::device);

	// Create two dummy objects for testing
	// Shark
	GameObject* shark = new GameObject(new vector3df(0, 20, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0) /*<-- Position, scale and rotation respectively*/,
										0, GameManager::smgr, -1111 /*<-- Parent, ISceneManager (GameManager's ISceneManager!) and instance ID*/,
										GameManager::smgr->getMesh("../media/shark.obj") /*<-- Mesh (optional)*/,
										GameManager::driver->getTexture("../media/Shark_Texture.jpg") /*<-- Texture (optional, mandatory if supplying mesh!)*/);

	// Rock
	GameObject* rock = new GameObject(new vector3df(0, 20, 0), new vector3df(20, 20, 20), new vector3df(0, 0, 0),
										0, GameManager::smgr, -1111,
										GameManager::smgr->getMesh("../media/rock.obj"),
										GameManager::driver->getTexture("../media/RockTexture.jpg"));


	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
		//!!Change parameters to correct type
		//Detect(player, win, key, shark);
		// Update our scene. gameManager.Update will also call Update for all GameObjects and their linked nodes
		gameManager.Update();
		player.updatePos();

		//check the boundaries
		camera.updatePos();

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