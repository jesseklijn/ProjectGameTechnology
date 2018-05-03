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
#include "GridMesh.h"
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
#include <iostream>
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

// Constants
const int NO_PARENT = 0;
const float KEYLIGHT_RADIUS = 50.f;
const float CHESTLIGHT_RADIUS = 90.f;

// Light data

//irr::video::SColorf ambientColor = irr::video::SColorf(0.2f, 0.2f, 0.2f, 0.2f);
// Debug Lighting
irr::video::SColorf ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);
const float FLASHLIGHT_RANGE = 1500.f;
vector3df chestLightOffset = vector3df(40,30,0);
vector3df keyLightOffset = vector3df(0, 20, 0);



// Create HUD object
HUD* hud = new HUD;

// Whether to hide or show the HUD
bool disableHud = false;

// Scenery generation
int critterCount = 300;
int shipCount = 6;
int rockCount = 60;
int ruinsCount = 25;
int coralCount = 75;
int plantCount = 25;
int skullCount = 1;
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
	//GameObject* groundPlane = new GameObject(new vector3df(0, -100, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
	//	0, GameManager::smgr, -1116,
	//	GameManager::smgr->getMesh("../media/plane.obj"),
	//	0, false);

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

	// Spawn player
	Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111);
	GameManager::gameObjects.push_back(player);

	// Attach flashlight to player
	ISceneNode* newPlayer = player;
	ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor, player->getAbsolutePosition(), GameManager::smgr->getActiveCamera()->getTarget(), FLASHLIGHT_RANGE, true, player);

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

	// Make a playingField (mesh out of grid)
	GameObject* playingField = new GridMesh(new vector3df(-GameManager::worldRadiusX - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE)/2), -200, -GameManager::worldRadiusZ - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE) / 2)), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
		GameManager::smgr->getRootSceneNode(), GameManager::smgr,-100, 0,0);

	// Spawn random objects on grid;
	// NOTE: Still being used by the key and chest only due to the extra features added compared to the scenary objects
	IMeshBuffer* planeBuffer = playingField->mesh->getMesh()->getMeshBuffer(0);

	// Get the vertices of the playingField 
	S3DVertex* mb_vertices = (S3DVertex*)planeBuffer->getVertices();

	// Amount of objects to be spawn on the grid
	int verticesGrid = planeBuffer->getVertexCount();

	// Keeps track what vertex has an object spawned on it
	vector<bool> spawnTracker(verticesGrid);

	// Key collectible object
	while (true)
	{
		// Generate a random number for the selection of a vertex so an object can get spawned on it
		int randomizer = rand() % planeBuffer->getVertexCount();
		// Checks if the vertex is free (no object has been drawn on the vertice)
		if (!spawnTracker[randomizer]) {
			GameObject* key = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y + 25,
				mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z), new vector3df(0.5, 0.5, 0.5), new vector3df(0, 0, 0),
				0, GameManager::smgr, 4,
				GameManager::smgr->getMesh("../media/key.obj"),
				GameManager::driver->getTexture("../media/RustTexture.jpg"));
			key->tag = GameObject::KEY;
			GameManager::gameObjects.push_back(key);
			ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), key->getPosition() + keyLightOffset, KEYLIGHT_RADIUS, false, NO_PARENT);
			break;
		}
	}

	// Win condition trigger object
		while (true)
		{
			// Generate a random number for the selection of a vertex so an object can get spawned on it
			int randomizer = rand() % planeBuffer->getVertexCount();
			// Checks if the vertex is free (no object has been drawn on the vertice)
			if (!spawnTracker[randomizer]) {
				GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
					0, GameManager::smgr, 5,
					GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
					GameManager::driver->getTexture("../media/GoldTexture.jpg"));
				chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
				chest->tag = GameObject::CHEST;
				ILightSceneNode* chestLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), chest->getPosition() + chestLightOffset, CHESTLIGHT_RADIUS, false, NO_PARENT);
				GameManager::gameObjects.push_back(chest);
				break;
			}
		}

	// Spawn rocks
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Rock.obj"); meshTextures.push_back("");


	// Adds objects on the vertices of the playingfield mesh 
	GridMesh::RandomObjectPlacementOnVertex(rockCount, playingField->getPosition(),
	                                      vector3df(150 + (rand() % 150) * 0.25f, 150 + (rand() % 150) * 0.25f,
	                                                150 + (rand() % 150) * 0.25f),
	                                      vector3df(rand() % 360, rand() % 360, rand() % 360), -1111,
	                                      meshDirectories, meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));
	//// Spawn ruins
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/ruinsArc.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsCathedral.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsFoundation.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsPillar.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsTempleRuin1.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/ruinsTempleRuin2.obj"); meshTextures.push_back("");

	GridMesh::RandomObjectPlacementOnVertex(ruinsCount, playingField->getPosition(), vector3df(1, 1, 1), vector3df(0, 0, 0),
	                                      -1111
	                                      , meshDirectories, meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));

	// Spawn corals
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Coral/coralBrain1.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralBrain2.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralBrain3.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Coral/coralPillar.obj"); meshTextures.push_back("");

	GridMesh::RandomObjectPlacementOnVertex(coralCount, playingField->getPosition(), vector3df(1, 1, 1), vector3df(0, 0, 0),
	                                      -1111
	                                      , meshDirectories, meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));


	// Spawn vines
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Plants/tiny_weed_03_01.obj"); meshTextures.push_back("");

	GridMesh::RandomObjectPlacementOnVertex(plantCount, playingField->getPosition(), vector3df(1, 1, 1), vector3df(0, 0, 0),
	                                      -1111
	                                      , meshDirectories, meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));

	// Spawn skulls
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/Bones/skullBig.obj"); meshTextures.push_back("");

	GridMesh::RandomObjectPlacementOnVertex(skullCount, playingField->getPosition(), vector3df(1, 1, 1), vector3df(0, 0, 0),
	                                      -1111
	                                      , meshDirectories, meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));

	// Spawn ship
	meshDirectories.clear();
	meshTextures.clear();
	meshDirectories.push_back("../media/ship.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/SailShip.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/Boat.obj"); meshTextures.push_back("");
	meshDirectories.push_back("../media/BoatWSail.obj"); meshTextures.push_back("");

	GridMesh::RandomObjectPlacementOnVertex(shipCount, playingField->getPosition(), {},
	                                      vector3df(rand() % 25, rand() % 10, rand() % 10), -1111 , meshDirectories,
	                                      meshTextures, playingField->mesh->getMesh()->getMeshBuffer(0));

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
		
		//Detect(newPlayer,
		//	chest->mesh,
		//	key->mesh,
		//	shark->mesh,
		//	itemPickedUp,
		//	GameManager::smgr);

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
