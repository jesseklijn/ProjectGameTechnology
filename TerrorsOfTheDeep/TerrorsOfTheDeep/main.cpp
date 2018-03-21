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
bool itemPickedUp[3] = {false, false, false };

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

	IGUIFont* font = GameManager::device->getGUIEnvironment()->getBuiltInFont();
	
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
	GameObject* shark = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/shark.obj")),
		GameManager::driver->getTexture("../media/Shark_Texture.jpg"));

	// Rock
	GameObject* rock = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/rock.obj")),
		GameManager::driver->getTexture("../media/RockTexture.jpg"),
		new const vector3df(-100, -50, -100),
		new const vector3df(20, 20, 20));

	// Rock
	GameObject* rock1 = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/rock.obj")),
		GameManager::driver->getTexture("../media/RockTexture.jpg"),
		new const vector3df(-200, -50, 75),
		new const vector3df(20, 20, 20));

	// Rock
	GameObject* rock2 = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/rock.obj")),
		GameManager::driver->getTexture("../media/RockTexture.jpg"),
		new const vector3df(0, -50, -25),
		new const vector3df(20, 20, 20));

	// Rock
	GameObject* rock3 = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/rock.obj")),
		GameManager::driver->getTexture("../media/RockTexture.jpg"),
		new const vector3df(-300, -50, 300),
		new const vector3df(20, 20, 20));

	// Rock
	GameObject* rock4 = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/rock.obj")),
		GameManager::driver->getTexture("../media/RockTexture.jpg"),
		new const vector3df(-375, -50, 205),
		new const vector3df(20, 20, 20));

	// key object
	GameObject* key = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/key.obj")),
		GameManager::driver->getTexture("../media/key.mtl"),
		new const vector3df(-175, -40, 100),
		new const vector3df(0.1, 0.1, 0.1));


	// win condition
	GameObject* win = new GameObject(GameManager::smgr->addAnimatedMeshSceneNode(GameManager::smgr->getMesh("../media/ChestCartoon.obj")),
		GameManager::driver->getTexture("../media/ChestCartoon.mtl"),
		new const vector3df(-250, -40, 300),
		new const vector3df(1, 1, 1));

	ISceneNode* newPlayer = &player;

	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
		//!!Change parameters to correct type
		Detect(newPlayer, 
			win->node, 
			key->node, 
			shark->node, 
			rock->node, 
			rock1->node, 
			rock2->node, 
			rock3->node, 
			rock4->node, 
			itemPickedUp, 
			GameManager::smgr);

		// Update our scene. gameManager.Update will also call Update for all GameObjects and their linked nodes
		gameManager.Update();
		player.updatePos();

		// Clear the HUD, update HUD values and prepare the updated HUD
		GameManager::guienv->clear();
		if (stamina >= 0 && stamina < 1000) {
			stamina++;
		}

		// Run the Draw() of the GameManager, which in turn also runs the Draw() for all GameObjects and their linked scene nodes
		gameManager.Draw();

		// We finished changing the scene
		// Now draw the scene in our actual window
		GameManager::smgr->drawAll();

		if (!disableHud) {
			hud->HudDraw(stamina, itemPickedUp, GameManager::driver, GameManager::guienv);
		}
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
