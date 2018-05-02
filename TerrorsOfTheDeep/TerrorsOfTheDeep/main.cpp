#pragma region Includes
#pragma once
#include <irrlicht.h>
#include "Lighting.h"
#include "HUD.h"
#include "Player.h"
#include "GridMesh.h"
#include "Camera.h"
#include "DetectCollision.h"
#include "Sound.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Critter.h"
#include "Shark.h"
#include <string>
#include <iostream>
#include "FlockingEntity.h"
#include <chrono>
#pragma endregion


#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
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
	SceneManager sceneManager;
	GameManager gameManager;
	GameManager::device->setWindowCaption(L"Terrors of the Deep");
	
	sceneManager.LoadScene(SceneManager::LEVEL);

	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Delta time start point
		auto frameTimeStart = std::chrono::system_clock::now();

		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));

		// Update our managers
		gameManager.Update();
		sceneManager.Update();

		// We finished changing the scene
		// Now draw the scene in our actual window
		GameManager::smgr->drawAll();

		// Clear the HUD, update HUD values and prepare the updated HUD
		GameManager::guienv->clear();

		// Run the Draw() of the GameManager, which in turn also runs the Draw() for all GameObjects and their linked scene nodes
		gameManager.Draw();
		sceneManager.Draw();

		// Finally, draw our HUD on the screen		
		GameManager::guienv->drawAll();

		// Our frame is finished
		GameManager::driver->endScene();

		// Delta time end point, calculate time passed for the next frame
		auto frameTimeEnd = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_seconds = frameTimeEnd - frameTimeStart;
		GameManager::deltaTime = elapsed_seconds.count() * GameManager::gameSpeed;
		GameManager::deltaTimeMS = GameManager::deltaTime * 1000.0f * GameManager::gameSpeed;
		GameManager::time += elapsed_seconds.count();
	}
	// Game end, drop our Irrlicht device
	GameManager::device->drop();

	// Stop sound
	sound_shutdown();

	return 0;
}