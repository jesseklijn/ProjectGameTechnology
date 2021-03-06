#pragma once
#include "SceneManager.h"
#include "Fader.h"
#include "GridMesh.h"
#include <Windows.h>

using namespace std;
static const int NO_PARENT = 0;
static const float KEYLIGHT_RADIUS = 2000.0f;
static const float CHESTLIGHT_RADIUS = 2000.0f;
static const float FLASHLIGHT_RANGE = 4000.0f;



GameObject* SceneManager::defaultGameObject;
vector<GameObject*> SceneManager::defaultGameObjectList;

SceneManager::SceneType SceneManager::scene = SceneType::NONE;
SceneManager::SceneType SceneManager::scenePrevious = SceneManager::scene;
SceneManager::FaderAction SceneManager::faderAction = FaderAction::NO_ACTION;
bool SceneManager::sceneIsPaused = false;
int meshSelector;

GameObject* SceneManager::levelMonster = nullptr;
GameObject* SceneManager::levelPlayer = nullptr;
IAnimatedMeshSceneNode* SceneManager::levelPlane = nullptr;
Menu* SceneManager::pauseMenu = nullptr;
Fader* SceneManager::fader = nullptr;
ITexture* SceneManager::loadingScreenImage = nullptr;
int SceneManager::groundID = -1337;

// Light data
irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(0.3f, 0.3f, 0.4f, 1.0f);
irr::video::SColorf SceneManager::ambientColorTitle = irr::video::SColorf(0.7f, 0.7f, 0.8f, 1.0f);
//irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
vector3df SceneManager::chestLightOffset = vector3df(40, 600, 0);
vector3df SceneManager::keyLightOffset = vector3df(0, 400, 0);

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
float SceneManager::introMouseOverlayDisplayTime = 12.0f * 1000.0f;
float SceneManager::introMouseOverlayDisplayTimer = -1.0f;
float SceneManager::introKeyOverlayTime = 1.0f * 1000.0f;
float SceneManager::introKeyOverlayTimer = -1.0f;
float SceneManager::introKeyOverlayDisplayTime = 10.0f * 1000.0f;
float SceneManager::introKeyOverlayDisplayTimer = -1.0f;

// Define default parameters
vector3df* SceneManager::vectorZero = new vector3df(0, 0, 0);
int SceneManager::noID = -1111;

float SceneManager::distanceKeyChest = 1500;

// Random scale and rotation setter for GameObjects
irr::core::vector3df SceneManager::baseScaleRock = vector3df(1, 1, 1);
irr::core::vector3df SceneManager::scaleVariationRock = vector3df(0.6f, 0.6f, 0.6f);
irr::core::vector3df SceneManager::rotationVariationRock = vector3df(45, 360, 45);

irr::core::vector3df SceneManager::baseScaleRuins = vector3df(2, 2, 2);
irr::core::vector3df SceneManager::scaleVariationRuins = vector3df(0, 0, 0);
irr::core::vector3df SceneManager::rotationVariationRuins = vector3df(5, 360, 5);

irr::core::vector3df SceneManager::baseScaleCorals = vector3df(2 , 2, 2);
irr::core::vector3df SceneManager::scaleVariationCorals = vector3df(0, 0, 0);
irr::core::vector3df SceneManager::rotationVariationCorals = vector3df(0, 0, 0);

irr::core::vector3df SceneManager::baseScaleVines = vector3df(1, 1.5, 1);
irr::core::vector3df SceneManager::scaleVariationVines = vector3df(0.25f, 0.5f, 0.25f);
irr::core::vector3df SceneManager::rotationVariationVines = vector3df(10, 360, 10);

irr::core::vector3df SceneManager::baseScaleShips = vector3df(2.5f, 2.5f, 2.5f);
irr::core::vector3df SceneManager::scaleVariationShips = vector3df(0.5f, 0.5f, 0.5f);
irr::core::vector3df SceneManager::rotationVariationShips = vector3df(45, 360, 45);

irr::core::vector3df SceneManager::baseScaleSkulls = vector3df(1, 1, 1);
irr::core::vector3df SceneManager::scaleVariationSkulls = vector3df(0, 0, 0);
irr::core::vector3df SceneManager::rotationVariationSkulls = vector3df(15, 360, 15);

// Constructor
SceneManager::SceneManager()
{
	scene = SceneType::NONE;
	scenePrevious = scene;
	sceneIsPaused = false;

	SceneManager::fader = new Fader(new vector2df(0, 0), new vector2df(1, 1), new vector2df(0, 0),
		0, GameManager::smgr, -1);
	fader->isFading = true;
}

// Destructor
SceneManager::~SceneManager()
{

}

/* Updates every frame.
Happens before any drawing.*/
void SceneManager::Update()
{
	if(SceneManager::scene == TITLE_SCREEN )
	{
		if (levelMonster != nullptr)
			levelMonster->Move(1000 * GameManager::deltaTime, vector3df(0, 0, 1), false);
	}
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
	keyOverlay->elementHeight = 350.0f;
	keyOverlay->elementSpacing = 16.0f;
	keyOverlay->hasWindowTitle = true;
	keyOverlay->windowTitle = "Touchdown!\n\nPress [W], [A], [S], [D] to move around\nPress [SPACE] to ascend, [CTRL] to descend.\nYou might want to watch out for terrors lurking in the deep darkness...\n\nHiding behind objects can make you harder to catch!";
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

void SceneManager::TriggerFaderAction()
{
	switch (SceneManager::faderAction)
	{
		case SceneManager::SCENE_SWITCH_TO_LEVEL:
		{
			SceneManager::LoadScene(SceneManager::LEVEL);
		} break;

		case SceneManager::SCENE_SWITCH_TO_MAIN:
		{
			SceneManager::PauseScene(false);			
			SceneManager::LoadScene(SceneManager::TITLE_SCREEN);
		} break;
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
	{
		GameManager::escaped = false;
		GameManager::gameOver = false;
	}
	GameManager::keyPickedUp = false;
	GameManager::smgr->clear();
	for (GameObject* gameObj : GameManager::gameObjects)
		if (gameObj != nullptr)
			delete gameObj;
	for (InterfaceObject* intObj : GameManager::interfaceObjects)
		if (intObj != nullptr)
			delete intObj;
	GameManager::gameObjects.clear();
	GameManager::interfaceObjects.clear();

	if (scene == TITLE_SCREEN && SceneManager::levelMonster != nullptr)
	{
		delete SceneManager::levelMonster;
		SceneManager::levelMonster = nullptr;
	}
	
	SceneManager::levelPlayer = nullptr;
	SceneManager::levelPlane = nullptr;
	SceneManager::mouseOverlay = nullptr;
	SceneManager::keyOverlay = nullptr;


	// Load the new scene
	scene = sceneToLoad;
	switch (scene)
	{
	case TITLE_SCREEN:
	{
		ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/WorldDetail/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);
		skydome->setMaterialFlag(EMF_FOG_ENABLE, true);
		
		sound_init();		
		background_music("../media/Sound/Main Menu/titlescreen.ogg");

		Lighting lighting = Lighting(GameManager::smgr);
		lighting.SetSceneLight(ambientColorTitle);
		GameManager::driver->setFog(SColor(1, 10, 10, 25), EFT_FOG_EXP2, 0.0f, 5000.0f, 0.0003f);

		camera = new Camera(GameManager::smgr);
		ICameraSceneNode* titleCam = GameManager::smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(90, 0, 0), 0, true);
		titleCam->setInputReceiverEnabled(false);

		Shark* shark = new Shark(new vector3df(3000, -500, -3500),
			new vector3df(0.5f, 0.5f, 0.5f),
			new vector3df(0, 0, 0),
			0,
			GameManager::smgr,
			-1111,
			GameManager::smgr->getMesh("../media/Monsters/Shark.obj"),
			0,
			false);
		SceneManager::levelMonster = shark;

		ISceneNode* title = GameManager::smgr->addBillboardSceneNode(0, dimension2d<f32>(59, 52), vector3df(50, 10, 0), 0, 0);
		title->setRotation(vector3df(0, 0, 0));
		title->setMaterialTexture(0, GameManager::driver->getTexture("../media/UI/titlescreen.png"));
		title->setMaterialFlag(EMF_LIGHTING, false);
		title->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

		Menu* TitleMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::PAUSE_MENU, 0, GameManager::smgr, 10000);
		TitleMenu->elementWidth = 200.0f;
		TitleMenu->hasWindow = false;
		TitleMenu->hasWindowBorder = false;
		TitleMenu->hasWindowTitle = false;
		TitleMenu->elementHeight = 124.0f;
		TitleMenu->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - TitleMenu->elementWidth / 2.0f,
			GameManager::screenDimensions.Height / 1.4f, 0.0f));
		GameManager::interfaceObjects.push_back(TitleMenu);

		int buttonCount = 2;
		float buttonWidth = TitleMenu->elementWidth - TitleMenu->elementSpacing * 2.0f;
		float buttonHeight = 50.0f;
		float buttonStartX = TitleMenu->getPosition().X + TitleMenu->elementWidth / 2.0f - buttonWidth / 2.0f;
		float buttonStartY = TitleMenu->getPosition().Y + TitleMenu->elementHeight - 1 -
			((buttonHeight + TitleMenu->elementSpacing) * 3);


		Button* startButton = new Button(new vector2df(buttonStartX, buttonStartY + ((buttonHeight + TitleMenu->elementSpacing))), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			(Button::ButtonType)((int)Button::MM_START), 0, GameManager::smgr, 15000);
		startButton->creator = TitleMenu;
		startButton->elementWidth = buttonWidth;
		startButton->elementHeight = buttonHeight;
		GameManager::interfaceObjects.push_back(startButton);

		Button* quitButton = new Button(new vector2df(buttonStartX, buttonStartY + ((buttonHeight + TitleMenu->elementSpacing)) * 2), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			(Button::ButtonType)((int)Button::MM_QUIT), 0, GameManager::smgr, 15000);
		quitButton->creator = TitleMenu;
		quitButton->elementWidth = buttonWidth;
		quitButton->elementHeight = buttonHeight;
		GameManager::interfaceObjects.push_back(quitButton);
	}
	break;

	case LOADING:
	{

	}
	break;


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

		Lighting lighting = Lighting(GameManager::smgr);
		lighting.SetSceneLight(ambientColor);
		GameManager::driver->setFog(SColor(1, 10, 10, 25), EFT_FOG_EXP2, 0.0f, 5000.0f, 0.0003f);
#pragma endregion

#pragma region World Generation
		std::vector<io::path> meshDirectories;
		std::vector<io::path> meshTextures;
		std::vector<io::path> loadingScreenImages;
		loadingScreenImages.push_back("../media/LoadingScreen/Backgrounds/BackgroundTerror1.png");
		loadingScreenImages.push_back("../media/LoadingScreen/Backgrounds/BackgroundTerror2.png");
		loadingScreenImages.push_back("../media/LoadingScreen/Backgrounds/BackgroundTerror3.png");
		loadingScreenImages.push_back("../media/LoadingScreen/Backgrounds/BackgroundTerror4.png");
		SceneManager::loadingScreenImage = GameManager::driver->getTexture(loadingScreenImages[rand() % loadingScreenImages.size()]);

		//Update loading screen
		StartLoadingScreen(SceneManager::BASICS);
		EndLoadingScreen();
		// Create a keymap-less camera for the intro sequence
		camera = new Camera(GameManager::smgr);
		IAnimatedMesh* playerMesh = GameManager::smgr->getMesh("../media/Player/FPSArms.obj");

		StartLoadingScreen(SceneManager::PLAYER);
		EndLoadingScreen();
		// Spawn player in cage
		Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0), 7.0f,
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
		StartLoadingScreen(SceneManager::TERRORS);
		EndLoadingScreen();
		// Attach flashlight to the player
		ISceneNode* newPlayer = player;
		ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor,
			player->getAbsolutePosition(),
			GameManager::smgr->getActiveCamera()->getTarget(),
			FLASHLIGHT_RANGE,
			true,
			player);

		Shark* shark = new Shark(new vector3df(5000, 3000, 5000),
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
		playingField->mesh->setID(SceneManager::groundID);
		playingField->isKinematic = true;
		SceneManager::levelPlane = playingField->mesh;

		ITriangleSelector* selector = GameManager::smgr->createTriangleSelector(playingField->mesh);
		playingField->mesh->setTriangleSelector(selector);
		selector->drop();

		// Spawn random objects on grid
		IMeshBuffer* planeBuffer = playingField->mesh->getMesh()->getMeshBuffer(0);

		StartLoadingScreen(SceneManager::RELICS);
		EndLoadingScreen();

		// Key collectible object
		GameObject* key = new GameObject(new vector3df(0, 0, 0), new vector3df(0.5, 0.5, 0.5), SceneManager::vectorZero,
			0, GameManager::smgr, 4,
			GameManager::smgr->getMesh("../media/WinLose/key.obj"),
			GameManager::driver->getTexture("../media/WinLose/RustTexture.jpg"));
		key->SetTag(GameObject::KEY);
		GameManager::gameObjects.push_back(key);
		


		// Adds objects on the vertices of the playingfield mesh 
		//playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, key, {}, true, true);

		// Win condition trigger object
		GameObject* chest = new GameObject(new vector3df(0, 0, 0), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
			0, GameManager::smgr, 5,
			GameManager::smgr->getMesh("../media/WinLose/ChestCartoon.obj"),
			GameManager::driver->getTexture("../media/WinLose/GoldTexture.jpg"));
		chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		chest->SetTag(GameObject::CHEST);
		
		GameManager::gameObjects.push_back(chest);

		// Adds objects on the vertices of the playingfield mesh 
		//playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, chest, {}, true, true);

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
				ILightSceneNode* chestLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), chest->getPosition() + chestLightOffset, CHESTLIGHT_RADIUS, false, 0);
				ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), key->getPosition() + keyLightOffset, KEYLIGHT_RADIUS, false, 0);
				break;
			}
		}
		StartLoadingScreen(SceneManager::CRITTERS);
		EndLoadingScreen();

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
		StartLoadingScreen(SceneManager::LAND_MARKS);
		EndLoadingScreen();
		// Spawn rocks
		meshDirectories.clear();
		meshTextures.clear();
		meshDirectories.push_back("../media/Rocks/Rock1.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Rocks/Rock2.obj"); meshTextures.push_back("");
		meshDirectories.push_back("../media/Rocks/PM_GraniteKnife_HR_Geometry.obj"); meshTextures.push_back("");

		// List contains rock game objects
		vector<GameObject*> rockList;
		//float scaleAdditionX = ((rand() % 100) * scaleVariationX) / 100.0f - scaleVariationX / 2.0f;
		//float scaleAdditionY = ((rand() % 100) * scaleVariationY) / 100.0f - scaleVariationY / 2.0f;
		//float scaleAdditionZ = ((rand() % 100) * scaleVariationZ) / 100.0f - scaleVariationZ / 2.0f;
		//float rotationAdditionX = ((rand() % 100) * rotationVariationX) / 100.0f - rotationVariationX / 2.0f;
		//float rotationAdditionY = ((rand() % 100) * rotationVariationY) / 100.0f - rotationVariationY / 2.0f;
		//float rotationAdditionZ = ((rand() % 100) * rotationVariationZ) / 100.0f - rotationVariationZ / 2.0f;

		// Creates x amount gameObjects 
		for (int i = 0; i < GameManager::rockCount; i++) {
			meshSelector = rand() % meshDirectories.size();
			vector3df* objectScale = new vector3df(
				1+ ((rand() % 100) * scaleVariationRock.X) / 100.0f - scaleVariationRock.X / 2.0f,
				1+ ((rand() % 100) * scaleVariationRock.Y) / 100.0f - scaleVariationRock.Y / 2.0f,
				1+ ((rand() % 100) * scaleVariationRock.Z) / 100.0f - scaleVariationRock.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				360 *((rand() % 100) * rotationVariationRock.X) / 100.0f - rotationVariationRock.X / 2.0f,
				360 *((rand() % 100) * rotationVariationRock.Y) / 100.0f - rotationVariationRock.Y / 2.0f,
				360 *((rand() % 100) * rotationVariationRock.Z) / 100.0f - rotationVariationRock.Z / 2.0f);

			GameObject* rock = new GameObject(SceneManager::vectorZero,  objectScale, objectRotation,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < rock->mesh->getMaterialCount(); mIndex++)
			{
				rock->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}
			GameManager::gameObjects.push_back(rock);
			rockList.push_back(rock);
		}
		StartLoadingScreen(SceneManager::RUINS);
		EndLoadingScreen();
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
			vector3df* objectScale = new vector3df(
			baseScaleRock.X	+ ((rand() % 100) * scaleVariationRock.X) / 100.0f - scaleVariationRock.X / 2.0f,
				baseScaleRock.Y + ((rand() % 100) * scaleVariationRock.Y) / 100.0f - scaleVariationRock.Y / 2.0f,
				baseScaleRock.Z + ((rand() % 100) * scaleVariationRock.Z) / 100.0f - scaleVariationRock.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				((rand() % 100) * rotationVariationRuins.X) / 100.0f - rotationVariationRuins.X / 2.0f,
				((rand() % 100) * rotationVariationRuins.Y) / 100.0f - rotationVariationRuins.Y / 2.0f,
				((rand() % 100) * rotationVariationRuins.Z) / 100.0f - rotationVariationRuins.Z / 2.0f);

			GameObject* ruins = new GameObject(SceneManager::vectorZero, objectScale, objectRotation,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < ruins->mesh->getMaterialCount(); mIndex++)
			{
				ruins->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}
			//GameManager::gameObjects.push_back(ruins);
			ruinsList.push_back(ruins);
		}
		StartLoadingScreen(SceneManager::CORALS);
		EndLoadingScreen();
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

			vector3df* objectScale = new vector3df(
				baseScaleCorals.X	+((rand() % 100) * scaleVariationCorals.X) / 100.0f - scaleVariationCorals.X / 2.0f,
				baseScaleCorals.Y	+((rand() % 100) * scaleVariationCorals.Y) / 100.0f - scaleVariationCorals.Y / 2.0f,
				baseScaleCorals.Z	+((rand() % 100) * scaleVariationCorals.Z) / 100.0f - scaleVariationCorals.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				((rand() % 100) * rotationVariationCorals.X) / 100.0f - rotationVariationCorals.X / 2.0f,
				((rand() % 100) * rotationVariationCorals.Y) / 100.0f - rotationVariationCorals.Y / 2.0f,
				((rand() % 100) * rotationVariationCorals.Z) / 100.0f - rotationVariationCorals.Z / 2.0f);

			GameObject* coral = new GameObject(SceneManager::vectorZero, objectScale, objectRotation,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < coral->mesh->getMaterialCount(); mIndex++)
			{
				coral->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}
			GameManager::gameObjects.push_back(coral);
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

			vector3df* objectScale = new vector3df(
				baseScaleVines.X	+((rand() % 100) * scaleVariationVines.X) / 100.0f - scaleVariationVines.X / 2.0f,
				baseScaleVines.Y	+((rand() % 100) * scaleVariationVines.Y) / 100.0f - scaleVariationVines.Y / 2.0f,
				baseScaleVines.Z	+((rand() % 100) * scaleVariationVines.Z) / 100.0f - scaleVariationVines.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				((rand() % 100) * rotationVariationVines.X) / 100.0f - rotationVariationVines.X / 2.0f,
				((rand() % 100) * rotationVariationVines.Y) / 100.0f - rotationVariationVines.Y / 2.0f,
				((rand() % 100) * rotationVariationVines.Z) / 100.0f - rotationVariationVines.Z / 2.0f);

			GameObject* vines = new GameObject(SceneManager::vectorZero, objectScale, objectRotation,
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

			vector3df* objectScale = new vector3df(
				baseScaleSkulls.X	+((rand() % 100) * scaleVariationSkulls.X) / 100.0f - scaleVariationSkulls.X / 2.0f,
				baseScaleSkulls.Y	+((rand() % 100) * scaleVariationSkulls.Y) / 100.0f - scaleVariationSkulls.Y / 2.0f,
				baseScaleSkulls.Z	+((rand() % 100) * scaleVariationSkulls.Z) / 100.0f - scaleVariationSkulls.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				((rand() % 100) * rotationVariationSkulls.X) / 100.0f - rotationVariationSkulls.X / 2.0f,
				((rand() % 100) * rotationVariationSkulls.Y) / 100.0f - rotationVariationSkulls.Y / 2.0f,
				((rand() % 100) * rotationVariationSkulls.Z) / 100.0f - rotationVariationSkulls.Z / 2.0f);

			GameObject* skull = new GameObject(SceneManager::vectorZero, objectScale, objectRotation,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < skull->mesh->getMaterialCount(); mIndex++)
			{
				skull->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}
			//GameManager::gameObjects.push_back(skull);
			skullList.push_back(skull);
		}
		StartLoadingScreen(SceneManager::SHIPS);
		EndLoadingScreen();
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

			vector3df* objectScale = new vector3df(
				baseScaleShips.X	+((rand() % 100) * scaleVariationShips.X) / 100.0f - scaleVariationShips.X / 2.0f,
				baseScaleShips.Y	+((rand() % 100) * scaleVariationShips.Y) / 100.0f - scaleVariationShips.Y / 2.0f,
				baseScaleShips.Z	+((rand() % 100) * scaleVariationShips.Z) / 100.0f - scaleVariationShips.Z / 2.0f);

			vector3df* objectRotation = new vector3df(
				360 * ((rand() % 100) * rotationVariationShips.X) / 100.0f - rotationVariationShips.X / 2.0f,
				360 * ((rand() % 100) * rotationVariationShips.Y) / 100.0f - rotationVariationShips.Y / 2.0f,
				360 * ((rand() % 100) * rotationVariationShips.Z) / 100.0f - rotationVariationShips.Z / 2.0f);

			GameObject* ship = new GameObject(SceneManager::vectorZero, objectScale, objectRotation,
				0, GameManager::smgr, SceneManager::noID,
				GameManager::smgr->getMesh(meshDirectories[meshSelector]));

			// Needs to be included to prevent textures being half transparent
			for (int mIndex = 0; mIndex < ship->mesh->getMaterialCount(); mIndex++)
			{
				ship->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			}
			//GameManager::gameObjects.push_back(ship);
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

		/*Initialize our background music*/
		sound_init();
		background_music("../media/Sound/Ambience/AmbientUnderwaterMaddnes.ogg");
#pragma endregion
	} break;

	case GAME_OVER:
	{
		//DetectCollision::ResetArray();
		Menu* gameOverMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::END_MENU, 0, GameManager::smgr, 10000);
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
	GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
	GameManager::guienv->clear();
	GameManager::driver->draw2DImage(SceneManager::loadingScreenImage, rect<s32>(0, 0, GameManager::screenDimensions.Width, GameManager::screenDimensions.Height), 
		rect<s32>(0, 0, SceneManager::loadingScreenImage->getOriginalSize().Width, SceneManager::loadingScreenImage->getOriginalSize().Height));
	ITexture* notifier = 0;
	switch (loadingType)
	{
		case SceneManager::BASICS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/basics.jpg"); break;

		case SceneManager::CREATURES:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Creatures.jpg"); break;

		case SceneManager::WORLD:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/World.jpg"); break;

		case SceneManager::LAND_MARKS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Landmarks.jpg"); break;

		case SceneManager::PLAYER:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/player.jpg"); break;

		case SceneManager::RELICS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/relics.jpg"); break;

		case SceneManager::CRITTERS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/critters.jpg"); break;

		case SceneManager::RUINS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/Ruins.jpg"); break;

		case SceneManager::SHIPS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/ships.jpg"); break;

		case SceneManager::VINES:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/vines.jpg"); break;

		case SceneManager::TERRORS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/terrors.jpg"); break;

		case SceneManager::CORALS:
			notifier = GameManager::driver->getTexture("../media/LoadingScreen/loadingTexts/coral.jpg"); break;

		default: break;
	}

	if (notifier != 0)
	{
		GameManager::guienv->addImage(notifier, core::position2d<s32>(GameManager::screenDimensions.Width / 2.0f - notifier->getOriginalSize().Width / 2.0f,
			GameManager::screenDimensions.Height - 100.0f));
	}
	GameManager::guienv->drawAll();
}

/* Ends the currently active loading screen.*/
void SceneManager::EndLoadingScreen()
{
	GameManager::driver->endScene();
	irrklang::ISound* bgMusic = GetBackgroundSound();
	if (bgMusic != nullptr)
		SetSoundVolume(bgMusic, bgMusic->getVolume() - 0.05f);
}
