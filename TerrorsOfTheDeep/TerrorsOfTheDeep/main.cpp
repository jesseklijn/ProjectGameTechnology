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
#include "InterfaceObject.h"
#include "Critter.h"
#include "Shark.h"
#include <string>
#include <iostream>
#include <chrono>
#include "Fader.h"
#pragma endregion


#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

bool requiresLoading = true;

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	/* Seed the random number generator, so we don't end up with
	the same random numbers on every run */
	srand(time(NULL));

	// Create managers
	GameManager gameManager;
	SceneManager sceneManager;
	DetectCollision detectCollision;

	GameManager::gameSeed = rand() % 100000;
	GameManager::device->setWindowCaption(L"Terrors Of The Deep");

	sceneManager.LoadScene(SceneManager::TITLE_SCREEN);

	////////// MAIN PROGRAM LOOP //////////
	while (GameManager::device->run())
	{
		// Delta time start point
		auto frameTimeStart = std::chrono::system_clock::now();

		// Begin the scene for this frame. It basically clears the buffers/screen with the given SColor
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));

		// Update the world
		sceneManager.Update();
		gameManager.Update();
		SceneManager::fader->Update();

		if (SceneManager::scene == SceneManager::LEVEL) 
			detectCollision.Detect(GameManager::smgr);
		else 
			detectCollision.ResetArray();

		// Draw the world
		GameManager::smgr->drawAll();
		GameManager::guienv->clear();
		sceneManager.Draw();
		gameManager.Draw();
		SceneManager::fader->DrawGUI();
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
	//Sound::SoundShutdown();
	sound_shutdown();
	return 0;
}
