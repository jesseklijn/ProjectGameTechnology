#pragma once
#include "SceneManager.h"
#include "GridMesh.h"
#include <Windows.h>

using namespace std;
static const int NO_PARENT = 0;
static const float KEYLIGHT_RADIUS = 1000.0f;
static const float CHESTLIGHT_RADIUS = 1000.0f;
static const float FLASHLIGHT_RANGE = 4000.0f;


GameObject* SceneManager::defaultGameObject;
vector<GameObject*> SceneManager::defaultGameObjectList;

SceneManager::SceneType SceneManager::scene = SceneManager::NONE;
SceneManager::SceneType SceneManager::scenePrevious = SceneManager::scene;
bool SceneManager::sceneIsPaused = false;
int meshSelector;

GameObject* SceneManager::levelMonster = nullptr;
GameObject* SceneManager::levelPlayer = nullptr;
IAnimatedMeshSceneNode* SceneManager::levelPlane = nullptr;
Menu* SceneManager::pauseMenu = nullptr;

// Light data
irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(0.3f, 0.3f, 0.4f, 1.0f);
//irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);
vector3df SceneManager::chestLightOffset = vector3df(40, 300, 0);
vector3df SceneManager::keyLightOffset = vector3df(0, 100, 0);

Camera* SceneManager::camera;
HUD* SceneManager::hud = new HUD();
bool SceneManager::disableHud = false;
vector3df SceneManager::rayStart = vector3df(0, 0, 0);
vector3df SceneManager::rayEnd = vector3df(0, 0, 0);
GameObject* SceneManager::divingCage = nullptr;

// Intro / Controls overlay
Menu* SceneManager::mouseOverlay = nullptr;
Menu* SceneManager::keyOverlay = nullptr;
bool SceneManager::introIsActive = false;
bool SceneManager::showControls = false;
bool SceneManager::showMouseOverlay = false;
bool SceneManager::showKeyOverlay = false;
bool SceneManager::cageShouldDescend = false;
float SceneManager::introCageDescendRate = 500.0f;
float SceneManager::introStartTime = 1.0f * 1000.0f;
float SceneManager::introStartTimer = -1.0f;
float SceneManager::introMouseOverlayTime = 1.0f * 1000.0f;
float SceneManager::introMouseOverlayTimer = -1.0f;
float SceneManager::introMouseOverlayDisplayTime = 10.0f * 1000.0f;
float SceneManager::introMouseOverlayDisplayTimer = -1.0f;
float SceneManager::introKeyOverlayTime = 1.0f * 1000.0f;
float SceneManager::introKeyOverlayTimer = -1.0f;
float SceneManager::introKeyOverlayDisplayTime = 10.0f * 1000.0f;
float SceneManager::introKeyOverlayDisplayTimer = -1.0f;

// Define default parameters
vector3df* SceneManager::vectorZero = new vector3df(0, 0, 0);
int SceneManager::noID = -1111;

float SceneManager::distanceKeyChest = 1500;

// Constructor
SceneManager::SceneManager()
{
	scene = NONE;
	scenePrevious = scene;
	sceneIsPaused = false;
}

// Destructor
SceneManager::~SceneManager()
{

}

/* Updates every frame.
Happens before any drawing.*/
void SceneManager::Update()
{
	if (GameManager::gameOver)
		SceneManager::LoadScene(GAME_OVER);

	// Pause menu trigger
	if (GameManager::eventManager.IsKeyPressed(KEY_ESCAPE) && scene == LEVEL)
		SceneManager::PauseScene(!sceneIsPaused);

	// Control overlay timers / intro management
	if (scene == LEVEL)
	{
		SceneManager::introStartTimer = (SceneManager::introStartTimer > 0.0f)
			? GameManager::Clamp(SceneManager::introStartTimer - GameManager::deltaTimeMS, 0.0f, SceneManager::introStartTime)
			: -1.0f;
		if (SceneManager::introStartTimer == 0.0f)
		{
			SceneManager::StartLevelIntro();
			SceneManager::introStartTimer = -1.0f;
		}

		bool countdownOverlayTimers = (introIsActive || showControls);
		if (countdownOverlayTimers)
		{
#pragma region Mouse overlay show timer
			SceneManager::introMouseOverlayTimer = (SceneManager::introMouseOverlayTimer > 0.0f)
				? GameManager::Clamp(SceneManager::introMouseOverlayTimer - GameManager::deltaTimeMS, 0.0f, SceneManager::introMouseOverlayTime)
				: -1.0f;
			if (SceneManager::introMouseOverlayTimer == 0.0f)
			{
				SceneManager::ShowMouseControlsOverlay();
				SceneManager::introMouseOverlayTimer = -1.0f;
			}
#pragma endregion

#pragma region Mouse overlay display timer
			SceneManager::introMouseOverlayDisplayTimer = (SceneManager::introMouseOverlayDisplayTimer > 0.0f)
				? GameManager::Clamp(SceneManager::introMouseOverlayDisplayTimer - GameManager::deltaTimeMS, 0.0f, SceneManager::introMouseOverlayDisplayTime)
				: -1.0f;
			if (SceneManager::introMouseOverlayDisplayTimer == 0.0f)
			{
				SceneManager::HideMouseControlsOverlay();
				SceneManager::introMouseOverlayDisplayTimer = -1.0f;
			}
#pragma endregion

#pragma region Key overlay show timer
			SceneManager::introKeyOverlayTimer = (SceneManager::introKeyOverlayTimer > 0.0f)
				? GameManager::Clamp(SceneManager::introKeyOverlayTimer - GameManager::deltaTimeMS, 0.0f, SceneManager::introKeyOverlayTime)
				: -1.0f;
			if (SceneManager::introKeyOverlayTimer == 0.0f)
			{
				SceneManager::ShowKeyControlsOverlay();
				SceneManager::introKeyOverlayTimer = -1.0f;
			}
#pragma endregion

#pragma region Key overlay display timer
			SceneManager::introKeyOverlayDisplayTimer = (SceneManager::introKeyOverlayDisplayTimer > 0.0f)
				? GameManager::Clamp(SceneManager::introKeyOverlayDisplayTimer - GameManager::deltaTimeMS, 0.0f, SceneManager::introKeyOverlayDisplayTime)
				: -1.0f;
			if (SceneManager::introKeyOverlayDisplayTimer == 0.0f)
			{
				// Timer trigger, done with all overlays
				SceneManager::HideKeyControlsOverlay();
				SceneManager::introIsActive = false;
				SceneManager::showControls = false;
				SceneManager::introKeyOverlayDisplayTimer = -1.0f;
			}
#pragma endregion

			// If this is the intro sequence, detect diving cage landing on sea floor
			if (introIsActive && divingCage != nullptr && cageShouldDescend)
			{
				GameManager::smgr->getActiveCamera()->setPosition(GameManager::smgr->getActiveCamera()->getPosition() + vector3df(0.0f, -introCageDescendRate * GameManager::deltaTime, 0.0f));
				SceneManager::divingCage->Move(introCageDescendRate * GameManager::deltaTime, vector3df(0.0f, -1.0f, 0.0f));

				rayStart = divingCage->getAbsolutePosition();
				rayEnd = rayStart + vector3df(0.0f, -100.0f, 0.0f);
				bool touchedDown = (GameManager::PerformRaycast(rayStart, rayEnd, SceneManager::levelPlane->getID()) != 0);
				if (touchedDown)
				{
					// Start the key overlay timer
					if (cageShouldDescend)
					{
						cageShouldDescend = false;
						GameManager::levelPlayer->canMove = true;
						SceneManager::introKeyOverlayTimer = SceneManager::introKeyOverlayTime;
					}
				}
			}
		}
	}


	if (camera)
		camera->updatePos();

}

void SceneManager::StartLevelIntro()
{
	if (introIsActive)
		return;

	introIsActive = true;
	cageShouldDescend = true;
	SceneManager::introMouseOverlayTimer = SceneManager::introMouseOverlayTime;
}

void SceneManager::EndLevelIntro()
{
	if (!introIsActive)
		return;

	introIsActive = false;
	SceneManager::introStartTimer = -1.0f;
	SceneManager::introMouseOverlayTimer = -1.0f;
	SceneManager::introMouseOverlayDisplayTimer = -1.0f;
	SceneManager::introKeyOverlayTimer = -1.0f;
	SceneManager::introKeyOverlayDisplayTimer = -1.0f;
}

void SceneManager::ShowMouseControlsOverlay()
{
	if (mouseOverlay != nullptr)
		SceneManager::HideMouseControlsOverlay();

	/* Creates the overlay as a Menu
	This simply uses the window functionality to act as an overlay */
	mouseOverlay = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
		Menu::OVERLAY, 0, GameManager::smgr, 10000, false);
	mouseOverlay->elementWidth = 335.0f;
	mouseOverlay->elementHeight = 175.0f;
	mouseOverlay->elementSpacing = 16.0f;
	mouseOverlay->hasWindowTitle = true;
	mouseOverlay->windowTitle = "Move the [MOUSE] to look around\n\nFind the key and open the treasure chest somewhere in this level to win";
	mouseOverlay->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - mouseOverlay->elementWidth / 2.0f,
		GameManager::screenDimensions.Height - mouseOverlay->elementHeight - mouseOverlay->elementSpacing, 0.0f));
	GameManager::interfaceObjects.push_back(mouseOverlay);

	// Starts the display timer for this overlay.
	SceneManager::introMouseOverlayDisplayTimer = SceneManager::introMouseOverlayDisplayTime;
}

void SceneManager::ShowKeyControlsOverlay()
{
	if (keyOverlay != nullptr)
		SceneManager::HideKeyControlsOverlay();

	/* Creates the overlay as a Menu
	This simply uses the window functionality to act as an overlay */
	keyOverlay = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
		Menu::OVERLAY, 0, GameManager::smgr, 10000, false);
	keyOverlay->elementWidth = 335.0f;
	keyOverlay->elementHeight = 200.0f;
	keyOverlay->elementSpacing = 16.0f;
	keyOverlay->hasWindowTitle = true;
	keyOverlay->windowTitle = "Touchdown!\n\nPress [W], [A], [S], [D] to move around\nYou might want to watch out for terrors lurking in the deep darkness...";
	keyOverlay->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - keyOverlay->elementWidth / 2.0f,
		GameManager::screenDimensions.Height - keyOverlay->elementHeight - keyOverlay->elementSpacing, 0.0f));
	GameManager::interfaceObjects.push_back(keyOverlay);

	// Starts the display timer for this overlay.
	SceneManager::introKeyOverlayDisplayTimer = SceneManager::introKeyOverlayDisplayTime;
}

void SceneManager::HideMouseControlsOverlay()
{
	if (mouseOverlay != nullptr)
	{
		// Clear GameManager tracking list entry
		int mIndex = GameManager::FindIndexInList<InterfaceObject>(mouseOverlay, GameManager::interfaceObjects);
		if (mIndex != -1)
			GameManager::interfaceObjects[mIndex] = nullptr;

		mouseOverlay->~Menu();
		mouseOverlay = nullptr;
	}
}

void SceneManager::HideKeyControlsOverlay()
{
	if (keyOverlay != nullptr)
	{
		// Clear GameManager tracking list entry
		int mIndex = GameManager::FindIndexInList<InterfaceObject>(keyOverlay, GameManager::interfaceObjects);
		if (mIndex != -1)
			GameManager::interfaceObjects[mIndex] = nullptr;

		keyOverlay->~Menu();
		keyOverlay = nullptr;

		if (introIsActive)
			SceneManager::EndLevelIntro();
	}
}

/* Updates every frame.
Happens after scene updates.*/
void SceneManager::Draw()
{
	if (!disableHud && hud)
		hud->HudDraw(GameManager::driver, GameManager::guienv);
}

SceneManager::Tag SceneManager::GetTag()
{
	return tag;
}

/* Loads the given scene. Unloads the currently active scene first. */
bool SceneManager::LoadScene(SceneType sceneToLoad)
{
	// Cancel if the scene we're trying to load is the same as the current scene
	if (sceneToLoad == scene || sceneToLoad == NONE)
		return false;

	// Unload current scene
	if (sceneToLoad != GAME_OVER)
		GameManager::gameOver = false;
	GameManager::smgr->clear();
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj != nullptr)
			delete gameObj;
	for (InterfaceObject* intObj : GameManager::interfaceObjects)
		if (intObj != nullptr)
			delete intObj;
	GameManager::gameObjects.clear();
	GameManager::interfaceObjects.clear();
	SceneManager::levelMonster = nullptr;
	SceneManager::levelPlayer = nullptr;
	SceneManager::levelPlane = nullptr;
	SceneManager::mouseOverlay = nullptr;
	SceneManager::keyOverlay = nullptr;


	//Update loading screen
	StartLoadingScreen(SceneManager::BASICS);
	EndLoadingScreen();

	// Load the new scene
	scene = sceneToLoad;
	switch (scene)
	{
	case TITLE_SCREEN:
	{

	} break;
	case LOADING:
	{

	} break;


	case LEVEL:
	{
#pragma region Setup
		GameManager::device->getCursorControl()->setVisible(false);

		/* Delta time start point
		Other than main(), it's also needed here to calculate the generation time
		to compensate for active timers. Otherwise most will instantly trigger
		when on the next frame 10000 ms has passed while generating for example */
		auto frameTimeStart = std::chrono::system_clock::now();

		// Set our skydome
		ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/WorldDetail/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);
		skydome->setMaterialFlag(EMF_FOG_ENABLE, true);

		// Initialize our background music
		sound_init();
		background_music("../media/AmbientUnderwaterMaddnes.ogg");


		Lighting lighting = Lighting(GameManager::smgr);
		lighting.SetSceneLight(ambientColor);
#pragma endregion

#pragma region World Generation
		std::vector<io::path> meshDirectories;
		std::vector<io::path> meshTextures;

		// Create a keymap-less camera for the intro sequence
		camera = new Camera(GameManager::smgr);
		IAnimatedMesh* playerMesh = GameManager::smgr->getMesh("../media/Player/FPSArms.obj");


		// Spawn player in cage
		Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0), 5,
			GameManager::smgr->getActiveCamera(), GameManager::smgr, -1337, playerMesh, GameManager::driver->getTexture("../media/Player/armsText.jpg"));
		GameManager::gameObjects.push_back(player);
		GameManager::levelPlayer = player;
		player->isKinematic = true;

		GameObject* cage = new GameObject(new vector3df(player->getParent()->getPosition().X, player->getParent()->getPosition().Y - 400.0f, player->getParent()->getPosition().Z),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0,
			GameManager::smgr,
			-1111,
			GameManager::smgr->getMesh("../media/Ruins/SharkCage.obj"));
		GameManager::gameObjects.push_back(cage);
		SceneManager::divingCage = cage;
		cage->isKinematic = true;

		// Attach flashlight to the player
		ISceneNode* newPlayer = player;
		ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor,
			player->getAbsolutePosition(),
			GameManager::smgr->getActiveCamera()->getTarget(),
			FLASHLIGHT_RANGE,
			true,
			player);

		Shark* shark = new Shark(new vector3df(8000, 5000, 0),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0,
			GameManager::smgr,
			-1111,
			GameManager::smgr->getMesh("../media/Monsters/Shark.obj"),
			0,
			false);
		SceneManager::levelMonster = shark;
		GameManager::gameObjects.push_back(shark);

		StartLoadingScreen(SceneManager::WORLD);
		EndLoadingScreen();
		// Make a playingField (mesh out of grid)
		GridMesh* playingField = new GridMesh(new vector3df(-GameManager::WORLD_RADIUS_X - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE) / 2),
			-300,
			-GameManager::WORLD_RADIUS_Z - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE) / 2)),
			new vector3df(1, 1, 1),
			new vector3df(0, 0, 0),
			0,
			GameManager::smgr,
			-100);
		GameManager::gameObjects.push_back(playingField);
		playingField->mesh->setID(-1337);
		playingField->isKinematic = true;
		SceneManager::levelPlane = playingField->mesh;

		ITriangleSelector* selector = GameManager::smgr->createTriangleSelector(playingField->mesh);
		playingField->mesh->setTriangleSelector(selector);
		selector->drop();

		// Spawn random objects on grid
		IMeshBuffer* planeBuffer = playingField->mesh->getMesh()->getMeshBuffer(0);

		// Key collectible object
		GameObject* key = new GameObject(new vector3df(0, 50, 0), new vector3df(0.5, 0.5, 0.5), SceneManager::vectorZero,
			0, GameManager::smgr, 4,
			GameManager::smgr->getMesh("../media/WinLose/key.obj"),
			GameManager::driver->getTexture("../media/WinLose/RustTexture.jpg"));
		key->setTag(GameObject::KEY);
		GameManager::gameObjects.push_back(key);
		ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), key->getPosition() + keyLightOffset, KEYLIGHT_RADIUS, false, nullptr);

		StartLoadingScreen(SceneManager::LAND_MARKS);
		EndLoadingScreen();
		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, key, {}, true, true);

		// Win condition trigger object
		GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
			0, GameManager::smgr, 5,
			GameManager::smgr->getMesh("../media/WinLose/ChestCartoon.obj"),
			GameManager::driver->getTexture("../media/WinLose/GoldTexture.jpg"));
		chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		chest->setTag(GameObject::CHEST);
		ILightSceneNode* chestLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), chest->getPosition() + chestLightOffset, CHESTLIGHT_RADIUS, false, nullptr);
		GameManager::gameObjects.push_back(chest);

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, chest, {}, true, true);

		// Keeps placing the key and chest on a different vertex if it's too close
		while (true)
		{
			float distance = key->getPosition().getDistanceFrom(chest->getPosition());
			if (distance < SceneManager::distanceKeyChest)
			{
				playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, key, {}, true, true);
				playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, chest, {}, true, true);
			}
			else
			{
				break;
			}
		}

		// Spawn critters
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Fish/Fish1.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish2.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish3.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish4.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish5.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish6.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish7.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Fish8.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/MantaRay.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/GoldenFish.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Fish/Rudd_Fish.obj"); meshTextures.push_back("");
		for (int critterIndex = 0; critterIndex < GameManager::critterCount; critterIndex++)
		{
			int graphicsIndex = rand() % meshDirectories.size();
			Critter* critter = new Critter(new vector3df(rand() % (GameManager::WORLD_RADIUS_X * 2) - GameManager::WORLD_RADIUS_X,
				rand() % GameManager::WORLD_RADIUS_Y,
				rand() % (GameManager::WORLD_RADIUS_Z * 2) - GameManager::WORLD_RADIUS_Z),
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

		// Spawn rocks
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Rocks/Rock1.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Rocks/Rock2.obj"); meshTextures.push_back("");
		//Bug: This file causes a memory allocation problem
		meshDirectories.push_back("../media/Rocks/PM_GraniteKnife_HR_Geometry.obj"); meshTextures.push_back("");

		// List contains rock game objects
		vector<GameObject*> rockList;

		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::rockCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* rock = new GameObject(SceneManager::vectorZero, new vector3df(1, 1, 1), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < rock->mesh->getMaterialCount(); mIndex++)
			{
				rock->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			rockList.push_back(rock);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), rockList);

		//// Spawn ruins
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Ruins/ruinsArc.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ruins/ruinsCathedral.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ruins/ruinsFoundation.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ruins/ruinsPillar.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ruins/ruinsTempleRuin1.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ruins/ruinsTempleRuin2.obj"); meshTextures.push_back("");

		// List contains ruins game objects
		vector<GameObject*> ruinsList;
		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::ruinsCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* ruins = new GameObject(SceneManager::vectorZero, new vector3df(0.2f, 0.2f, 0.2f), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < ruins->mesh->getMaterialCount(); mIndex++)
			{
				ruins->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			ruinsList.push_back(ruins);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), ruinsList);

		// Uses the meshDirectionaries and meshTexture list to create game objects (coral)
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Coral/coralBrain1.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Coral/coralBrain2.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Coral/coralBrain3.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Coral/coralPillar.obj"); meshTextures.push_back("");

		// List contains coral game objects
		vector<GameObject*> coralList;

		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::coralCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* coral = new GameObject(SceneManager::vectorZero, new vector3df(1, 1, 1), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < coral->mesh->getMaterialCount(); mIndex++)
			{
				coral->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			coralList.push_back(coral);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), coralList);

		// Uses the meshDirectionaries and meshTexture list to create game objects (vines)
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Plants/tiny_weed_03_01.obj"); meshTextures.push_back("");

		// List contains vine game objects
		vector<GameObject*> vinesList;
		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::plantCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* vines = new GameObject(SceneManager::vectorZero, new vector3df(1, 1, 1), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < vines->mesh->getMaterialCount(); mIndex++)
			{
				vines->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			vinesList.push_back(vines);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), vinesList);

		// Uses the meshDirectionaries and meshTexture list to create game objects (skull)
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Bones/skullBig.obj"); meshTextures.push_back("");

		// List contains skull game objects
		vector<GameObject*> skullList;

		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::skullCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* skull = new GameObject(SceneManager::vectorZero, new vector3df(0.25f, 0.25f, 0.25f), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < skull->mesh->getMaterialCount(); mIndex++)
			{
				skull->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			skullList.push_back(skull);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), skullList);
		// Uses the meshDirectionaries and meshTexture list to create game objects (ship)
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Ships/ship.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ships/ShipCom.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ships/ShipRow.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Ships/submarine.obj"); meshTextures.push_back("");

		// List contains ship game objects
		vector<GameObject*> shipList;

		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::shipCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			GameObject* ship = new GameObject(SceneManager::vectorZero, new vector3df(0.5f, 0.5f, 0.5f), SceneManager::vectorZero,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < ship->mesh->getMaterialCount(); mIndex++)
			{
				ship->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}

			shipList.push_back(ship);
		}

		// Adds objects on the vertices of the playingfield mesh 
		playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), shipList);

		// Calculate how long the total level generation and setup took until this point
		auto frameTimeEnd = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_seconds = frameTimeEnd - frameTimeStart;
		float generationDuration = elapsed_seconds.count() * GameManager::gameSpeed  * 1000.0f;

		// Start the controls display timers, compensate for the huge deltaTime of 1-frame generation
		SceneManager::introStartTimer = generationDuration + SceneManager::introStartTime;
#pragma endregion
	} break;

	case GAME_OVER:
	{
		Menu* gameOverMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::PAUSE_MENU, 0, GameManager::smgr, 10000);
		gameOverMenu->elementWidth = 200.0f;
		gameOverMenu->elementHeight = 124.0f;
		gameOverMenu->hasWindowTitle = false;
		gameOverMenu->hasBackground = true;
		gameOverMenu->background = GameManager::driver->getTexture("../media/UI/YouLose.png");
		gameOverMenu->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - gameOverMenu->elementWidth / 2.0f,
			GameManager::screenDimensions.Height / 2.0f - gameOverMenu->elementHeight / 2.0f, 0.0f));
		GameManager::interfaceObjects.push_back(gameOverMenu);

		int buttonCount = 2;
		float buttonWidth = gameOverMenu->elementWidth - gameOverMenu->elementSpacing * 2.0f;
		float buttonHeight = 50.0f;
		float buttonStartX = gameOverMenu->getPosition().X + gameOverMenu->elementWidth / 2.0f - buttonWidth / 2.0f;
		float buttonStartY = gameOverMenu->getPosition().Y + gameOverMenu->elementHeight - 1 -
			((buttonHeight + gameOverMenu->elementSpacing) * buttonCount);

		for (int bIndex = 0; bIndex < buttonCount; bIndex++)
		{
			Button* button = new Button(new vector2df(buttonStartX, buttonStartY + ((buttonHeight + gameOverMenu->elementSpacing) * bIndex)), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
				(Button::ButtonType)((int)Button::GO_RETRY + bIndex), 0, GameManager::smgr, 15000);
			button->creator = gameOverMenu;
			button->elementWidth = buttonWidth;
			button->elementHeight = buttonHeight;
			GameManager::interfaceObjects.push_back(button);
		}
	} break;

	default:
	{
		return false;
	} break;
	}

	SceneManager::OnSceneChange();
	return true;
}

/* Pauses the current scene by setting the game speed multiplier to 0. */
void SceneManager::PauseScene(bool mode)
{
	// Cancel if our new mode is the same as our current paused state
	if (mode == sceneIsPaused)
		return;

	// Pause / unpause game
	GameManager::gameSpeed = mode ? 0.0f : 1.0f;
	GameManager::smgr->getActiveCamera()->setInputReceiverEnabled(!mode);
	GameManager::device->getCursorControl()->setVisible(mode);
	sceneIsPaused = mode;

	// Toggle pause menu
	if (sceneIsPaused)
	{
		pauseMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::PAUSE_MENU, 0, GameManager::smgr, 10000);
		pauseMenu->elementWidth = 200.0f;
		pauseMenu->elementHeight = 250.0f;
		pauseMenu->windowTitle = "Pause Menu";
		pauseMenu->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - pauseMenu->elementWidth / 2.0f,
			GameManager::screenDimensions.Height / 2.0f - pauseMenu->elementHeight / 2.0f, 0.0f));
		GameManager::interfaceObjects.push_back(pauseMenu);

		int buttonCount = 3;
		float buttonWidth = pauseMenu->elementWidth - pauseMenu->elementSpacing * 2.0f;
		float buttonHeight = 50.0f;
		float buttonStartX = pauseMenu->getPosition().X + pauseMenu->elementWidth / 2.0f - buttonWidth / 2.0f;
		float buttonStartY = pauseMenu->getPosition().Y + pauseMenu->elementHeight - 1 -
			((buttonHeight + pauseMenu->elementSpacing) * buttonCount);

		for (int bIndex = 0; bIndex < buttonCount; bIndex++)
		{
			Button* button = new Button(new vector2df(buttonStartX, buttonStartY + ((buttonHeight + pauseMenu->elementSpacing) * bIndex)), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
				(Button::ButtonType)bIndex, 0, GameManager::smgr, 15000);
			button->creator = pauseMenu;
			button->elementWidth = buttonWidth;
			button->elementHeight = buttonHeight;
			GameManager::interfaceObjects.push_back(button);
		}
	}
	else
	{




		// Delete the pause menu
		if (pauseMenu != nullptr)
		{
			// Clear GameManager tracking list entry
			int mIndex = GameManager::FindIndexInList<InterfaceObject>(pauseMenu, GameManager::interfaceObjects);
			if (mIndex != -1)
				GameManager::interfaceObjects[mIndex] = nullptr;
			pauseMenu->~Menu();
		}
	}
}

/* Triggers whenever a scene switch happens.*/
void SceneManager::OnSceneChange()
{
	// std::cout << "OnSceneChange, from " << (int)scenePrevious << " to " << (int)scene << std::endl;
	scenePrevious = scene;
}

/* Creates a loading screen effect.
Used to smoothly load a scene before showing it.*/
void SceneManager::StartLoadingScreen(LoadingType loadingType)
{

	//TODO: Loading screen!
	GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
	GameManager::guienv->clear();
	GameManager::device->setWindowCaption(L"Loading Terrors Of The Deep");


	IGUIImage* image = GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/backgrounds/ruins3000x2000.jpg"), core::position2d<s32>(0, 0), false, 0, -1, L"test");
	cout << image->getAbsoluteClippingRect().LowerRightCorner.X << endl;
	cout << image->getAbsoluteClippingRect().LowerRightCorner.Y << endl;

	//GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingTitle.png"), core::position2d<s32>((GameManager::screenDimensions.Width - 1036) / 2, 0), true, 0, -1, L"test");
	//GameManager::guienv->addImage(GameManager::driver->getTexture("../media/underwater-ruins.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 700) / 2, (GameManager::screenDimensions.Height - 454) / 1.5F	), true, 0, -1, L"test");

	if (loadingType == SceneManager::BASICS) {
		GameManager::device->setWindowCaption(L"Loading BASICS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/basics.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::CREATURES)
	{
		GameManager::device->setWindowCaption(L"Loading CREATURES Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Creatures.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::WORLD)
	{
		GameManager::device->setWindowCaption(L"Loading WORLD Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/World.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::LAND_MARKS)
	{
		GameManager::device->setWindowCaption(L"Loading LAND_MARKS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Landmarks.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::PLAYER)
	{
		GameManager::device->setWindowCaption(L"Loading PLAYER Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/player.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::RELICS)
	{
		GameManager::device->setWindowCaption(L"Loading RELIC Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/relics.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::CRITTERS)
	{
		GameManager::device->setWindowCaption(L"Loading CRITTERS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/critters.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::RUINS)
	{
		GameManager::device->setWindowCaption(L"Loading RUINS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Ruins.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::SHIPS)
	{
		GameManager::device->setWindowCaption(L"Loading SHIPS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/ships.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::VINES)
	{
		GameManager::device->setWindowCaption(L"Loading VINES Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/vines.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::TERRORS)
	{
		GameManager::device->setWindowCaption(L"Loading TERRORS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/terrors.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::SHIPS)
	{
		GameManager::device->setWindowCaption(L"Loading SHIPS Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/ships.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	else if (loadingType == SceneManager::CORALS)
	{
		GameManager::device->setWindowCaption(L"Loading CORAL Terrors Of The Deep");
		GameManager::guienv->addImage(GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/coral.jpg"), core::position2d<s32>((GameManager::screenDimensions.Width - 579) / 2, (GameManager::screenDimensions.Height - 93)), true, 0, -1, L"test");

	}
	GameManager::guienv->drawAll();

}

/* Ends the currently active loading screen.*/
void SceneManager::EndLoadingScreen()
{
	GameManager::driver->endScene();
}
