#pragma once
#include "SceneManager.h"

static const int NO_PARENT = 0;
static const float KEYLIGHT_RADIUS = 50.f;
static const float CHESTLIGHT_RADIUS = 90.f;
static const float FLASHLIGHT_RANGE = 1500.f;

SceneManager::SceneType SceneManager::scene = SceneManager::NONE;
SceneManager::SceneType SceneManager::scenePrevious = SceneManager::scene;
bool SceneManager::sceneIsPaused = false;

// Light data
irr::video::SColorf SceneManager::ambientColor = irr::video::SColor(255, 30, 30, 50);
//irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);
vector3df SceneManager::chestLightOffset = vector3df(40, 30, 0);
vector3df SceneManager::keyLightOffset = vector3df(0, 20, 0);

Camera* SceneManager::camera;
HUD* SceneManager::hud = new HUD();
bool SceneManager::disableHud = false;

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
	if (camera)
		camera->updatePos();

	if (GameManager::eventManager.IsKeyPressed(KEY_ESCAPE) && scene == LEVEL) 
		SceneManager::PauseScene(!sceneIsPaused);
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
	if (sceneToLoad == scene)
		return false;

	// Unload current scene
	GameManager::smgr->clear();
	for (GameObject* gameObj : GameManager::gameObjects)
		delete gameObj;
	GameManager::gameObjects.clear();

	// Load the new scene
	scene = sceneToLoad;
	switch (scene) 
	{
		case TITLE_SCREEN:
		{

		} break;

		case LEVEL:
		{
			#pragma region Setup
			GameManager::device->getCursorControl()->setVisible(false);

			// Set our skydome
			ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);
			skydome->setMaterialFlag(EMF_FOG_ENABLE, true);

			// Initialize our background music
			sound_init();
			background_music("../media/AmbientUnderwaterMaddnes.ogg");

			// Adds the camera and binds the keys to the camera's movement
			camera = new Camera(GameManager::smgr);
			Lighting lighting = Lighting(GameManager::smgr);
			lighting.SetSceneLight(ambientColor);
			#pragma endregion

			#pragma region World Generation
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

			// Spawn world objects
			IAnimatedMesh* playerMesh = GameManager::smgr->getMesh("../media/FPSArms.obj");

			// Player
			Player* player = new Player(new vector3df(0, -0.8, 3), new vector3df(4, 4, 4), new vector3df(180, 0, 180),
				GameManager::smgr->getActiveCamera(), GameManager::smgr, 9000, playerMesh,
				GameManager::driver->getTexture("../media/armsText"));
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

			FlockingEntity* flockOfFish = new FlockingEntity(new vector3df(100, -80, 100), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				GameManager::smgr->getRootSceneNode(), GameManager::smgr, -500, GameManager::smgr->getMesh("../media/FishSpawn.obj"),
				GameManager::driver->getTexture("../media/GoldTexture.jpg"));
			flockOfFish->tag = GameObject::CREATURE;
			GameManager::gameObjects.push_back(flockOfFish);

			// Make a playingField (mesh out of grid)
			GameObject* playingField = new GridMesh(new vector3df(-GameManager::WORLD_RADIUS_X, -200, -GameManager::WORLD_RADIUS_Z), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				GameManager::smgr->getRootSceneNode(), GameManager::smgr, -100, 0, 0);

			// Spawn random objects on grid;
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
				// Generate a random number for the selection of a vertice so an object can get spawned on it
				int randomizer = rand() % planeBuffer->getVertexCount();
				// Checks if the vertice is free (no object has been drawn on the vertice)
				if (!spawnTracker[randomizer]) {
					GameObject* key = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y + 25,
						mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z), new vector3df(0.5, 0.5, 0.5), new vector3df(0, 0, 0),
						0, GameManager::smgr, 4,
						GameManager::smgr->getMesh("../media/key.obj"),
						GameManager::driver->getTexture("../media/RustTexture.jpg"));
					key->setTag(GameObject::KEY);
					GameManager::gameObjects.push_back(key);
					ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), key->getPosition() + keyLightOffset, KEYLIGHT_RADIUS, false, nullptr);
					break;
				}
			}

			// Win condition trigger object
			while (true)
			{
				// Generate a random number for the selection of a vertice so an object can get spawned on it
				int randomizer = rand() % planeBuffer->getVertexCount();
				// Checks if the vertice is free (no object has been drawn on the vertice)
				if (!spawnTracker[randomizer]) {
					GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
						0, GameManager::smgr, 5,
						GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
						GameManager::driver->getTexture("../media/GoldTexture.jpg"));
					chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
					chest->setTag(GameObject::CHEST);
					ILightSceneNode* chestLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), chest->getPosition() + chestLightOffset, CHESTLIGHT_RADIUS, false, nullptr);
					GameManager::gameObjects.push_back(chest);
					break;
				}
			}

			// Spawn rocks
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Rock.obj"); meshTextures.push_back("");
			for (int i = 0; i < GameManager::rockCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer])
					{
						int typeIndex = rand() % meshDirectories.size();
						GameObject* rock = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(150 + (rand() % 150) * 0.25f, 150 + (rand() % 150) * 0.25f, 150 + (rand() % 150) * 0.25f),
							new vector3df(rand() % 360, rand() % 360, rand() % 360),
							0, GameManager::smgr, -1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							0);
						for (int mIndex = 0; mIndex < rock->mesh->getMaterialCount(); mIndex++)
						{
							rock->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
						}
						GameManager::gameObjects.push_back(rock);
						spawnTracker[randomizer] = true;
						break;
					}
				}
			}

			//// Spawn ruins
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/ruinsArc.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsCathedral.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsFoundation.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsPillar.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsTempleRuin1.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsTempleRuin2.obj"); meshTextures.push_back("");
			for (size_t i = 0; i < GameManager::ruinsCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer])
					{
						int typeIndex = rand() % meshDirectories.size();
						GameObject* ruin = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(1, 1, 1),
							new vector3df(0, 0, 0),
							0, GameManager::smgr, -1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							0);
						GameManager::gameObjects.push_back(ruin);
						spawnTracker[randomizer] == true;
						break;
					}
				}
			}
			// Spawn corals
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Coral/coralBrain1.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralBrain2.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralBrain3.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralPillar.obj"); meshTextures.push_back("");

			for (size_t i = 0; i < GameManager::coralCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer]) {
						int typeIndex = rand() % meshDirectories.size();
						GameObject* coral = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(1, 1, 1),
							new vector3df(0, 0, 0),
							0, GameManager::smgr, -1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							0);
						for (int mIndex = 0; mIndex < coral->mesh->getMaterialCount(); mIndex++)
							coral->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
						GameManager::gameObjects.push_back(coral);
						spawnTracker[randomizer] == true;
						break;
					}
				}
			}


			// Spawn vines
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Plants/tiny_weed_03_01.obj"); meshTextures.push_back("");

			for (size_t i = 0; i < GameManager::plantCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer]) {
						int typeIndex = rand() % meshDirectories.size();
						GameObject* vines = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(1, 1, 1),
							new vector3df(0, 0, 0),
							0, GameManager::smgr, -1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							0);
						for (int mIndex = 0; mIndex < vines->mesh->getMaterialCount(); mIndex++)
							vines->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
						GameManager::gameObjects.push_back(vines);
						spawnTracker[randomizer] == true;
						break;
					}
				}
			}

			// Spawn skulls
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Bones/skullBig.obj"); meshTextures.push_back("");

			for (size_t i = 0; i < GameManager::skullCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer]) {
						int typeIndex = rand() % meshDirectories.size();
						GameObject* skull = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(1, 1, 1),
							new vector3df(0, 0, 0),
							0, GameManager::smgr, -1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							0);
						skull->mesh->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
						GameManager::gameObjects.push_back(skull);
						spawnTracker[randomizer] == true;
						break;
					}
				}
			}

			// Spawn ship
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/ship.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/SailShip.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Boat.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/BoatWSail.obj"); meshTextures.push_back("");

			for (size_t i = 0; i < GameManager::shipCount; i++)
			{
				while (true) {
					// Generate a random number for the selection of a vertice so an object can get spawned on it
					int randomizer = rand() % planeBuffer->getVertexCount();
					// Checks if the vertice is free (no object has been drawn on the vertice)
					if (!spawnTracker[randomizer]) {
						int typeIndex = rand() % meshDirectories.size();
						GameObject* ship = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + playingField->getPosition().X, mb_vertices[randomizer].Pos.Y + playingField->getPosition().Y,
							mb_vertices[randomizer].Pos.Z + playingField->getPosition().Z),
							new vector3df(1, 1, 1),
							new vector3df(rand() % 25, rand() % 10, rand() % 10),
							0,
							GameManager::smgr,
							-1111,
							GameManager::smgr->getMesh(meshDirectories[typeIndex]),
							meshTextures[typeIndex] != "" ? GameManager::driver->getTexture(meshTextures[typeIndex]) : 0);
						for (int mIndex = 0; mIndex < ship->mesh->getMaterialCount(); mIndex++)
							ship->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
						GameManager::gameObjects.push_back(ship);
						spawnTracker[randomizer] == true;
						break;
					}
				}
			}
		#pragma endregion
		} break;

		case GAME_OVER:
		{

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
	sceneIsPaused = mode;

	// Toggle pause menu
	if (sceneIsPaused)
	{
		Menu* pauseMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::PAUSE_MENU, 0, GameManager::smgr, 10000);
		pauseMenu->elementWidth = 200.0f;
		pauseMenu->elementHeight = 250.0f;
		pauseMenu->windowTitle = "Pause Menu";
		pauseMenu->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - pauseMenu->elementWidth / 2.0f,
			GameManager::screenDimensions.Height / 2.0f - pauseMenu->elementHeight / 2.0f, 0.0f));
		GameManager::interfaceObjects.push_back(pauseMenu);

		int buttonCount = 2;
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
		Menu* pauseMenu = (Menu*)GameManager::FindGameObjectWithTag<InterfaceObject>(DynamicUpdater::INTERFACE_MENU, GameManager::interfaceObjects);
		if (pauseMenu)
			pauseMenu->~Menu();
	}
}

/* Triggers whenever a scene switch happens.*/
void SceneManager::OnSceneChange()
{
	std::cout << "OnSceneChange, from " << (int)scenePrevious << " to " << (int)scene << std::endl;
	scenePrevious = scene;
}

/* Creates a loading screen effect.
Used to smoothly load a scene before showing it.*/
void SceneManager::StartLoadingScreen()
{
	// TODO: Loading screen!
}

/* Ends the currently active loading screen.*/
void SceneManager::EndLoadingScreen()
{
	// TODO: Loading screen!
}